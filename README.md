# AlouFEM
# please follow this guideline to set up AlouFEM before you start to use it
1. if you want to use Cmake to build solution for AlouFEM, do as follows:
	(1) copy these 3 libraries onto your hard disk: NetCDF_DIR, BOOST_ROOT, ExodusII_DIR.
	(2) add their paths (directory of the folder named "include") as environment variables of system individually, shown like this:
	|	variable  		|	value												|
	|	NetCDF_DIR 		| 	C:\AlouFEM_contrib\AlouFEM_Lib\NetCDF\4.7.4			|
	|	BOOST_ROOT		|	C:\AlouFEM_contrib\AlouFEM_Lib\boost_1_75_0			|
	|	ExodusII_DIR 	| 	C:\AlouFEM_contrib\AlouFEM_Lib\ExodusII\install		|
	(3) if Cmake is already open, you need to close it and reopen Cmake to load previous setups.
2. if you want to generate documentation extracted from codes (relevant projects: "extract", "user_doc"), make sure you have Python3 and a TeX distribution (e.g. MiKTeX) correctly installed on your computer.