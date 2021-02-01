//   file PLANSTRN.CXX
 
#include "planstrn.hxx"
#include "node.hxx"
#include "material.hxx"
#include "gausspnt.hxx"
#include "polynoxy.hxx"
#include "flotmtrx.hxx"
#include "diagmtrx.hxx"
#include "flotarry.hxx"
#include "intarray.hxx"
#include "domain.hxx"
#include <math.h>
#include <stdio.h>


PlaneStrain :: PlaneStrain (int n, Domain* aDomain)
	     : Element (n,aDomain)
   // Constructor.
{
   numberOfNodes  = 4 ;
   jacobianMatrix = NULL ;
   this -> computeGaussPoints() ;
}

//begin{usr_doc}
//begin{comment}
/*
In order to cumpute the element strain-displacement materix $\boldsymbol{B}$ at a gauss point $(r,s)$,
we can first calculate the Jacobian matrix of this element as J\_rs in the code. 

Then the inverse Jacobian matrix can be obtained as J\_inv\_rs in the code.
as defined in the Eq. \ref{eq:Jacobi_inv_2D} The elements of this iverse Jacobian matrix can be expressed as:
\begin{equation} \label{eq:Jacobi_inv_2D_1}
\boldsymbol{J}^{-1}
=
\begin{bmatrix}
\frac{\partial r}{\partial x} & \frac{\partial r}{\partial y}\\
\frac{\partial s}{\partial x} & \frac{\partial s}{\partial y}
\end{bmatrix}
=
\begin{bmatrix}
J^{-1}_{11} & J^{-1}_{12}\\
J^{-1}_{21} & J^{-1}_{22}
\end{bmatrix}
\end{equation}

%As in the code, r\_x=(*J\_inv\_rs)(1)[1], r\_y = (*J\_inv\_rs)(1)[2], s\_x = (*J\_inv\_rs)(2)[1] and s\_y = (*J\_inv\_rs)(2)[2].

The shape functions used is given in Eq. \ref{eq:shapefunction_quad}, 
so we can derive the partial derivatives of the shape function in terms of the coordinates $(r,s)$, as in Eq. \ref{eq:h_rs_quad}:
\begin{subequations}
  \label{eq:h_rs_quad}
  \begin{align}
    \label{eq:h1_r_quad}
    \frac{\partial h_1}{\partial r}=\frac{1}{4}(1+s) \\
    \label{eq:h2_r_quad}
    \frac{\partial h_2}{\partial r}=-\frac{1}{4}(1+s) \\
		\label{eq:h3_r_quad}
		\frac{\partial h_3}{\partial r}=-\frac{1}{4}(1-s) \\
		\label{eq:h4_r_quad}
		\frac{\partial h_4}{\partial r}=\frac{1}{4}(1-s) \\
		\label{eq:h1_s_quad}
		\frac{\partial h_1}{\partial s}=\frac{1}{4}(1+r) \\
		\label{eq:h2_s_quad}
		\frac{\partial h_2}{\partial s}=\frac{1}{4}(1-r) \\
		\label{eq:h3_s_quad}
		\frac{\partial h_3}{\partial s}=-\frac{1}{4}(1-r) \\
		\label{eq:h4_s_quad}
		\frac{\partial h_4}{\partial s}=-\frac{1}{4}(1+r) \\
  \end{align}
\end{subequations}


As the interpolation of the displacement defined in Eq. \ref{eq:disp_interpolation}, 
we can have the displacement interpolation for this 2D quadrilateral element, as expressed in Eq. \ref{eq:disp_interpolation_quad}:
\begin{subequations}
  \label{eq:disp_interpolation_quad}
  \begin{align}
    \label{eq:disp_interpolation_quad_u}
    & u=\sum^{4}_{i=1}h_iu_i \\
    \label{eq:disp_interpolation_quad_v}
    & v=\sum^{3}_{i=1}h_iv_i 
  \end{align}
\end{subequations}


The partial derivatives of the displacement in terms of the coordinates $(r,s)$ can be obtained by differentiating the Eq. \ref{eq:disp_interpolation_quad}:
\begin{equation}
\label{eq:partial_derivatives_disp_rs}
\begin{bmatrix}
\frac{\partial u}{\partial r} \\
\frac{\partial v}{\partial r} \\
\frac{\partial u}{\partial s} \\
\frac{\partial v}{\partial s}
\end{bmatrix}
=
\begin{bmatrix}
\frac{\partial h_1}{\partial r}u_1+\frac{\partial h_2}{\partial r}u_2+\frac{\partial h_3}{\partial r}u_3+\frac{\partial h_4}{\partial r}u_4 \\
\frac{\partial h_1}{\partial r}v_1+\frac{\partial h_2}{\partial r}v_2+\frac{\partial h_3}{\partial r}v_3+\frac{\partial h_4}{\partial r}v_4 \\
\frac{\partial h_1}{\partial s}u_1+\frac{\partial h_2}{\partial s}u_2+\frac{\partial h_3}{\partial s}u_3+\frac{\partial h_4}{\partial s}u_4 \\
\frac{\partial h_1}{\partial s}v_1+\frac{\partial h_2}{\partial s}v_2+\frac{\partial h_3}{\partial s}v_3+\frac{\partial h_4}{\partial s}v_4
\end{bmatrix}
=
\begin{bmatrix}
\frac{\partial h_1}{\partial r}&0&\frac{\partial h_2}{\partial r}&0&\frac{\partial h_3}{\partial r}&0&\frac{\partial h_4}{\partial r}&0 \\
0&\frac{\partial h_1}{\partial r}&0&\frac{\partial h_2}{\partial r}&0&\frac{\partial h_3}{\partial r}&0&\frac{\partial h_4}{\partial r} \\
\frac{\partial h_1}{\partial s}&0&\frac{\partial h_2}{\partial s}&0&\frac{\partial h_3}{\partial s}&0&\frac{\partial h_4}{\partial s}&0 \\
0&\frac{\partial h_1}{\partial s}&0&\frac{\partial h_2}{\partial s}&0&\frac{\partial h_3}{\partial s}&0&\frac{\partial h_4}{\partial s}
\end{bmatrix}
\begin{bmatrix}
u_1 \\
v_1 \\
u_2 \\
v_2 \\
u_3 \\
v_3 \\
u_4 \\
v_4
\end{bmatrix}
\end{equation}

Substitute Eq. \ref{eq:partial_derivatives_disp_rs} into the 2D strain tensor definition (Eq. \ref{eq:strain_def}), we have:
\begin{equation}
\label{eq:strain_def_quad}
\boldsymbol{\varepsilon}=
\begin{bmatrix}
\frac{\partial r}{\partial x} & 0 & \frac{\partial s}{\partial x} & 0 \\
0 & \frac{\partial r}{\partial y} & 0 & \frac{\partial s}{\partial y} \\
\frac{\partial r}{\partial y} & \frac{\partial r}{\partial x} & \frac{\partial s}{\partial y} & \frac{\partial s}{\partial x}
\end{bmatrix}
\begin{bmatrix}
\frac{\partial h_1}{\partial r}&0&\frac{\partial h_2}{\partial r}&0&\frac{\partial h_3}{\partial r}&0&\frac{\partial h_4}{\partial r}&0 \\
0&\frac{\partial h_1}{\partial r}&0&\frac{\partial h_2}{\partial r}&0&\frac{\partial h_3}{\partial r}&0&\frac{\partial h_4}{\partial r} \\
\frac{\partial h_1}{\partial s}&0&\frac{\partial h_2}{\partial s}&0&\frac{\partial h_3}{\partial s}&0&\frac{\partial h_4}{\partial s}&0 \\
0&\frac{\partial h_1}{\partial s}&0&\frac{\partial h_2}{\partial s}&0&\frac{\partial h_3}{\partial s}&0&\frac{\partial h_4}{\partial s}
\end{bmatrix}
\begin{bmatrix}
u_1 \\
v_1 \\
u_2 \\
v_2 \\
u_3 \\
v_3 \\
u_4 \\
v_4
\end{bmatrix}
=\boldsymbol{B}\boldsymbol{\hat{u}}
\end{equation}

in which,
\begin{equation}
\label{eq:B_matrix_def_quad}
\boldsymbol{B}=
\begin{bmatrix}
B_{11} & 0      & B_{13} & 0      & B_{15} & 0      & B_{17}  & 0 \\
0      & B_{22} & 0      & B_{24} & 0      & B_{26} & 0      & B_{28} \\
B_{31} & B_{32} & B_{33} & B_{34} & B_{35} & B_{36} & B_{37} & B_{38}
\end{bmatrix}
\end{equation}
and
\begin{equation}
\label{eq:u_hat_matrix_def_quadral}
\boldsymbol{\hat{u}}^{T}=
\begin{bmatrix}
u_1 & v_1 & u_2 & v_2 & u_3 & v_3 & u_4 & v_4
\end{bmatrix}
\end{equation}

$\boldsymbol{B}$ in Eq. \ref{eq:B_matrix_def_quad} is the displacement-strain transformation matrix of 2D quadrilateral element,
whose elements are expressed as in Eq. \ref{eq:B_elements_quad}:
\begin{subequations}
  \label{eq:B_elements_quad}
  \begin{align}
    B_{11}=\frac{\partial h_1}{\partial r}\frac{\partial r}{\partial x}+\frac{\partial h_1}{\partial s}\frac{\partial s}{\partial x} \\
    B_{13}=\frac{\partial h_2}{\partial r}\frac{\partial r}{\partial x}+\frac{\partial h_2}{\partial s}\frac{\partial s}{\partial x} \\
    B_{15}=\frac{\partial h_3}{\partial r}\frac{\partial r}{\partial x}+\frac{\partial h_3}{\partial s}\frac{\partial s}{\partial x} \\
    B_{17}=\frac{\partial h_4}{\partial r}\frac{\partial r}{\partial x}+\frac{\partial h_4}{\partial s}\frac{\partial s}{\partial x} \\
    B_{22}=\frac{\partial h_1}{\partial r}\frac{\partial r}{\partial y}+\frac{\partial h_1}{\partial s}\frac{\partial s}{\partial y} \\
    B_{24}=\frac{\partial h_2}{\partial r}\frac{\partial r}{\partial y}+\frac{\partial h_2}{\partial s}\frac{\partial s}{\partial y} \\
    B_{26}=\frac{\partial h_3}{\partial r}\frac{\partial r}{\partial y}+\frac{\partial h_3}{\partial s}\frac{\partial s}{\partial y} \\
    B_{28}=\frac{\partial h_4}{\partial r}\frac{\partial r}{\partial y}+\frac{\partial h_4}{\partial s}\frac{\partial s}{\partial y} \\
    B_{31}=\frac{\partial h_1}{\partial r}\frac{\partial r}{\partial y}+\frac{\partial h_1}{\partial s}\frac{\partial s}{\partial y} \\
    B_{32}=\frac{\partial h_1}{\partial r}\frac{\partial r}{\partial x}+\frac{\partial h_1}{\partial s}\frac{\partial s}{\partial x} \\
    B_{33}=\frac{\partial h_2}{\partial r}\frac{\partial r}{\partial y}+\frac{\partial h_2}{\partial s}\frac{\partial s}{\partial y} \\
    B_{34}=\frac{\partial h_2}{\partial r}\frac{\partial r}{\partial x}+\frac{\partial h_2}{\partial s}\frac{\partial s}{\partial x} \\
    B_{35}=\frac{\partial h_3}{\partial r}\frac{\partial r}{\partial y}+\frac{\partial h_3}{\partial s}\frac{\partial s}{\partial y} \\
    B_{36}=\frac{\partial h_3}{\partial r}\frac{\partial r}{\partial x}+\frac{\partial h_3}{\partial s}\frac{\partial s}{\partial x} \\
    B_{37}=\frac{\partial h_4}{\partial r}\frac{\partial r}{\partial y}+\frac{\partial h_4}{\partial s}\frac{\partial s}{\partial y} \\
    B_{38}=\frac{\partial h_4}{\partial r}\frac{\partial r}{\partial x}+\frac{\partial h_4}{\partial s}\frac{\partial s}{\partial x}
  \end{align}
\end{subequations}

Now we can compute the value of all elements in matrix $\boldsymbol{B}$ at the Gauss Point $(r,s)$, 
by substituting the values in Eq. \ref{Jacobi_inv_2D_1} and Eq. \ref{h_rs_quad} into Eq. \ref{B_elements_quad}.

*/
//end{comment}
//begin{code}
FloatMatrix*  PlaneStrain::ComputeBmatrixAt (GaussPoint *aGaussPoint)
   // Returns the [3x8] strain-displacement matrix {B} of the receiver, eva-
   // luated at aGaussPoint.
{
   FloatMatrix *J_rs,*J_inv_rs;
   FloatArray  *coord ;
   double      r,s,r_x,r_y,s_x,s_y,
	       h1_r,h2_r,h3_r,h4_r,h1_s,h2_s,h3_s,h4_s ;

   // natural coordinates r and s :
   coord = aGaussPoint -> giveCoordinates() ;
   r   = coord -> at(1) ;
   s   = coord -> at(2) ;

   // partial derivatives of r and s, with respect to x and y :
   J_rs = this -> giveJacobianMatrix() -> EvaluatedAt(coord) ;
	 J_rs->printYourself();
   J_inv_rs   = J_rs -> GiveInverse() ;

   r_x = (*J_inv_rs)(1)[1];
   r_y = (*J_inv_rs)(1)[2];
	 s_x = (*J_inv_rs)(2)[1];
   s_y = (*J_inv_rs)(2)[2];

   // hi is the ith shape function of the element, hi_r or hi_s are the partial derivatives of the shape function in terms of the coordinate (r,s)
   h1_r = (-1. + s) * 0.25 ;
   h2_r = ( 1. - s) * 0.25 ;
   h3_r = ( 1. + s) * 0.25 ;
   h4_r = (-1. - s) * 0.25 ;
   h1_s = (-1. + r) * 0.25 ;
   h2_s = (-1. - r) * 0.25 ;
   h3_s = ( 1. + r) * 0.25 ;
   h4_s = ( 1. - r) * 0.25 ;

   // B matrix  -  3 rows : epsilon-X, epsilon-Y, gamma-XY  :
	 FloatMatrix* B = new FloatMatrix(3,8) ;
   (*B)(1)[1] = h1_r*r_x + h1_s*s_x ;
	 (*B)(1)[3] = h2_r*r_x + h2_s*s_x ;
	 (*B)(1)[5] = h3_r*r_x + h3_s*s_x ;
	 (*B)(1)[7] = h4_r*r_x + h4_s*s_x ;
   (*B)(2)[2] = h1_r*r_y + h1_s*s_y ;
   (*B)(2)[4] = h2_r*r_y + h2_s*s_y ;
   (*B)(2)[6] = h3_r*r_y + h3_s*s_y ;
   (*B)(2)[8] = h4_r*r_y + h4_s*s_y ;
   (*B)(3)[1] = h1_r*r_y + h1_s*s_y ;
   (*B)(3)[2] = h1_r*r_x + h1_s*s_x ;
   (*B)(3)[3] = h2_r*r_y + h2_s*s_y ;
   (*B)(3)[4] = h2_r*r_x + h2_s*s_x ;
   (*B)(3)[5] = h3_r*r_y + h3_s*s_y ;
   (*B)(3)[6] = h3_r*r_x + h3_s*s_x ;
   (*B)(3)[7] = h4_r*r_y + h4_s*s_y ;
   (*B)(3)[8] = h4_r*r_x + h4_s*s_x ;
  
   delete J_rs ;
   delete J_inv_rs ;

   return B ;
}
//end{code}
//end{usr_doc}
   
FloatMatrix*  PlaneStrain :: computeConstitutiveMatrix ()
   // Returns the [3x3] elasticity matrix {E} of the receiver.
{
   Material *mat ;
   double   e,nu,ee,shear ;

   mat   = this -> giveMaterial() ;
   e     = mat -> give('E') ;
   nu    = mat -> give('n') ;
   ee    = e / ((1.+nu) * (1.-nu-nu)) ;
   shear = e / (2.+nu+nu) ;
   constitutiveMatrix = new FloatMatrix(3,3) ;

   constitutiveMatrix->at(1,1) = (1.-nu) * ee ;
   constitutiveMatrix->at(1,2) =     nu  * ee ;
   constitutiveMatrix->at(2,1) =     nu  * ee ;
   constitutiveMatrix->at(2,2) = (1.-nu) * ee ;
   constitutiveMatrix->at(3,3) =  shear ;

   return constitutiveMatrix ;
}


void  PlaneStrain :: computeGaussPoints ()
   // Sets up the array containing the four Gauss points of the receiver.
{
   FloatArray *coord1,*coord2,*coord3,*coord4 ;
   double     a,aMinus ;

   a                   = 1. / sqrt(3.) ;
   aMinus              = -a ;
   numberOfGaussPoints = 4 ;
   gaussPointArray     = new GaussPoint* [numberOfGaussPoints] ;

   coord1             = new FloatArray(2) ;
   coord1 -> at(1)    = aMinus ;
   coord1 -> at(2)    = aMinus ;
   gaussPointArray[0] = new GaussPoint(this,1,coord1,1.) ;

   coord2             = new FloatArray(2) ;
   coord2 -> at(1)    = a ;
   coord2 -> at(2)    = aMinus ;
   gaussPointArray[1] = new GaussPoint(this,2,coord2,1.) ;

   coord3             = new FloatArray(2) ;
   coord3 -> at(1)    = a ;
   coord3 -> at(2)    = a ;
   gaussPointArray[2] = new GaussPoint(this,3,coord3,1.) ;

   coord4             = new FloatArray(2) ;
   coord4 -> at(1)    = aMinus ;
   coord4 -> at(2)    = a ;
   gaussPointArray[3] = new GaussPoint(this,4,coord4,1.) ;
}


FloatMatrix*  PlaneStrain :: ComputeNmatrixAt (GaussPoint* aGaussPoint)
   // Returns the displacement interpolation matrix {N} of the receiver, eva-
   // luated at aGaussPoint.
{
   double       ksi,eta,n1,n2,n3,n4 ;
   FloatMatrix* answer ;

   ksi = aGaussPoint -> giveCoordinate(1) ;
   eta = aGaussPoint -> giveCoordinate(2) ;
   n1  = (1. - ksi) * (1. - eta) * 0.25 ;
   n2  = (1. + ksi) * (1. - eta) * 0.25 ;
   n3  = (1. + ksi) * (1. + eta) * 0.25 ;
   n4  = (1. - ksi) * (1. + eta) * 0.25 ;
   answer = new FloatMatrix(2,8) ;

   answer->at(1,1) = n1 ;
   answer->at(1,3) = n2 ;
   answer->at(1,5) = n3 ;
   answer->at(1,7) = n4 ;
   answer->at(2,2) = n1 ;
   answer->at(2,4) = n2 ;
   answer->at(2,6) = n3 ;
   answer->at(2,8) = n4 ;

   return answer ;
}


double  PlaneStrain :: computeVolumeAround (GaussPoint* aGaussPoint)
   // Returns the portion of the receiver which is attached to aGaussPoint.
{
   FloatMatrix* jacob ;
   FloatArray*  coord ;
   double       determinant,weight,thickness,volume ;

   coord       = aGaussPoint -> giveCoordinates() ;
   jacob       = this -> giveJacobianMatrix() -> EvaluatedAt(coord) ;
   determinant = fabs (jacob->giveDeterminant()) ;
   weight      = aGaussPoint -> giveWeight() ;
   thickness   = this -> giveMaterial() -> give('t') ;

   volume      = determinant * weight * thickness ;

   delete jacob ;
   return volume ;
}

//begin{usr_doc}
//begin{comment}
/*
In order to cumpute the 2D quadrilateral element Jacobi matrix $\boldsymbol{J}$ as defined in Eq. \ref{eq:Jacobi_2D}, 
we use the coordinate interpolation in Eq.\ref{eq:crd_interpolation} and the shape function $h_1$, $h_2$, $h_3$ and $h_4$
defined in Eq.\ref{eq:shapefunction_quad}, writing the coordinates as following:

\begin{subequations}
  \label{eq:crd_quadral}
  \begin{align}
		\label{eq:crd_quadral_x}
		& x=\frac{1}{4}(1+r)(1+s)x_1+\frac{1}{4}(1-r)(1+s)x_2+\frac{1}{4}(1-r)(1-s)x_3+\frac{1}{4}(1+r)(1-s)x_4 &\\
		\label{eq:crd_quadral_y}
		& y=\frac{1}{4}(1+r)(1+s)y_1+\frac{1}{4}(1-r)(1+s)y_2+\frac{1}{4}(1-r)(1-s)y_3+\frac{1}{4}(1+r)(1-s)y_4
  \end{align}
\end{subequations}

As defined in Eq. \ref{eq:Jacobi_2D}, the Jacobian matrix of a 2D element is a $2 \times 2$ matrix: 
\begin{equation}
  \boldsymbol{J}=
  \begin{bmatrix} \notag
    \frac{\partial x}{\partial r} & \frac{\partial y}{\partial r}\\
    \frac{\partial x}{\partial s} & \frac{\partial y}{\partial s}
  \end{bmatrix}
	=
	\begin{bmatrix} \notag
	j_{11} & j_{21} \\
	j_{12} & j_{22}
	\end{bmatrix}
\end{equation}

we have the expression of each element in the Jacobian matrix as a polynomial function of $(r,s)$
\begin{subequations}
\notag
\begin{align}
  j_{11}&=\frac{\partial x}{\partial r}=\frac{1}{4}(x_1-x_2-x_3+x_4) & \,&+ &           0\qquad\cdot\qquad&r & +&\, & \frac{1}{4}(x_1-x_2+x_3-x_4)&s\\
  j_{12}&=\frac{\partial x}{\partial s}=\frac{1}{4}(x_1+x_2-x_3-x_4) & \,&+ & \frac{1}{4}(x_1-x_2+x_3-x_4)&r & +&\, &           0\qquad\cdot\qquad&s\\
  j_{21}&=\frac{\partial y}{\partial r}=\frac{1}{4}(y_1-y_2-y_3+y_4) & \,&+ &           0\qquad\cdot\qquad&r & +&\, & \frac{1}{4}(y_1-y_2+y_3-y_4)&s\\
  j_{22}&=\frac{\partial y}{\partial s}=\frac{1}{4}(y_1+y_2-y_3-y_4) & \,&+ & \frac{1}{4}(x_1-x_2+x_3-x_4)&r & +&\, &           0\qquad\cdot\qquad&s
\end{align}

When implement in the code, we store the coefficient of the polynomial function in Jacobian matrix, so $J_{ij}$ becomes to be vectors:
\begin{align}
j_{11}&=\frac{\partial x}{\partial r}=[\frac{1}{4}(x_1-x_2-x_3+x_4), 0, \frac{1}{4}(x_1-x_2+x_3-x_4)] \\
j_{12}&=\frac{\partial x}{\partial s}=[\frac{1}{4}(x_1+x_2-x_3-x_4), \frac{1}{4}(x_1-x_2+x_3-x_4), 0] \\
j_{21}&=\frac{\partial y}{\partial r}=[\frac{1}{4}(y_1-y_2-y_3+y_4), 0, \frac{1}{4}(y_1-y_2+y_3-y_4)] \\
j_{22}&=\frac{\partial y}{\partial s}=[\frac{1}{4}(y_1+y_2-y_3-y_4), \frac{1}{4}(y_1-y_2+y_3-y_4), 0]
\end{align}
\end{subequations}

*/
//end{comment}
//begin{code}
PolynomialMatrix*  PlaneStrain::giveJacobianMatrix()
{
  Node         *node1, *node2, *node3, *node4;
  PolynomialXY *j11, *j12, *j21, *j22;
  double       x1, x2, x3, x4, y1, y2, y3, y4;
  
  if (!jacobianMatrix) {
    node1 = this->giveNode(1);
    node2 = this->giveNode(2);
    node3 = this->giveNode(3);
    node4 = this->giveNode(4);
    x1 = node1->giveCoordinate(1);
    x2 = node2->giveCoordinate(1);
    x3 = node3->giveCoordinate(1);
    x4 = node4->giveCoordinate(1);
    y1 = node1->giveCoordinate(2);
    y2 = node2->giveCoordinate(2);
    y3 = node3->giveCoordinate(2);
    y4 = node4->giveCoordinate(2);
    
    j11 = new PolynomialXY(1);
    j11->at(1) = (x1 - x2 - x3 + x4) * 0.25;
    j11->at(2) = 0.;
    j11->at(3) = (x1 - x2 + x3 - x4) * 0.25;
    
    j12 = new PolynomialXY(1);
    j12->at(1) = (x1 + x2 - x3 - x4) * 0.25;
    j12->at(2) = (x1 - x2 + x3 - x4) * 0.25;
    j12->at(3) = 0.;
    
    j21 = new PolynomialXY(1);
    j21->at(1) = (y1 - y2 - y3 + y4) * 0.25;
    j21->at(2) = 0.;
    j21->at(3) = (y1 - y2 + y3 - y4) * 0.25;
    
    j22 = new PolynomialXY(1);
    j22->at(1) = (y1 + y2 - y3 - y4) * 0.25;
    j22->at(2) = (y1 - y2 + y3 - y4) * 0.25;
    j22->at(3) = 0.;
    
    jacobianMatrix = new PolynomialMatrix(2, 2);
    jacobianMatrix->at(1, 1) = j11;
    jacobianMatrix->at(1, 2) = j12;
    jacobianMatrix->at(2, 1) = j21;
    jacobianMatrix->at(2, 2) = j22;
  }
  
  return jacobianMatrix;
}
//end{code}
//end{usr_doc}


