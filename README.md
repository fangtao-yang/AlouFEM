# AlouFEM
# please follow this guideline to set up AlouFEM before you start to use it
# concerning libraries and packages mentionned here, you can find a copy of what you need under AlouFEM_contrib/AlouFEM_Lib, in case your computer is not connected to internet

1. if you want to use Cmake to build solution for AlouFEM, you need to add a few missing libraries:
	(1) copy these 3 libraries onto your hard disk: NetCDF_DIR, BOOST_ROOT, ExodusII_DIR.
	(2) add their paths (directory of the folder named "include") as environment variables of system individually, shown like this:
	|	variable  		|	value								|
	|	NetCDF_DIR 		| 	C:\AlouFEM_contrib\AlouFEM_Lib\NetCDF\4.7.4			|
	|	BOOST_ROOT		|	C:\AlouFEM_contrib\AlouFEM_Lib\boost_1_75_0			|
	|	ExodusII_DIR 		| 	C:\AlouFEM_contrib\AlouFEM_Lib\ExodusII\install			|
	(3) if Cmake is already open, you need to close it and reopen Cmake to load previous setups.


2. if you want to generate documentation extracted from codes (by building project "user_doc"), you need to set up Python3 and LaTeX:
	(1) install Python3 on your computer (make sure you have write permission to its installation directory)
	(2) install "pygments" package using pip tool provided by Python (cmd: pip install pygments); if your computer is not connected to internet, go to AlouFEM_contrib/AlouFEM_Lib/pygments to find what you need 
	(3) add installation paths concerning python and pygments to environment variable of system "Path":
		C:\Program Files\Python39\
		C:\Program Files\Python39\Scripts\
		C:\Program Files\Python39\Lib\site-packages
	(4) install a TeX distribution (e.g. MiKTeX) on your computer
	(5) download "minted" package for LaTeX and put it locally under installation_directory_MiKTeX/tex/latex; if your computer is not connected to internet, go to AlouFEM_contrib/AlouFEM_Lib/minted to find what you need
