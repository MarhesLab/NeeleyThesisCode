************************
*** NeeleyThesisCode ***
************************

Hi! My name is Wil Neeley, and this is the code I used to complete my Masters thesis. This involved developing a quadrotor control system within a Vicon motion capture system with a custom ground control station program, with an AscTec Hummingbird quadrotor programmed for direct motor speed control, and with all the controls/state estimation taking place on an Intel Edison embedded Linux computer. The overwhelming majority of the code found here is in C++, though some C code or some MATLAB/Python utility files can be found in a few places. 


To understand a lot of what I was doing, it's probably best that you consult my thesis first. I'll include a copy in this repository, but here is its full title in case it somehow vanishes. UNM's LoboVault should have it once it gets officially released.

	Title: Design and Development of a High-Performance Quadrotor Control Architecture 
			Based on Feedback Linearization
	Author: William Neeley
	Submission Date: July 27th, 2015
	

As for the code, if what I'm doing is confusing or you don't understand something, just remember that there's no shame in Googling things. That's how about 63.7% of this code was written. I made that statistic up. Still, you get the point. If Googling doesn't yield any answers, you can send me an email at my UNM email address, which I'll try to keep checking.

	Email address: wjneeley at unm dot edu


Here is a basic list of the sections found in this file:

  - Learn Linux
  - Folder Descriptions
  - Makefiles
  - Setup Process 



*******************
*** Learn Linux ***
*******************

To work with the code I've created, you really need to know and understand how to work with Linux, especially through the terminal. There's not really any way to get around it. Fortunately, I stumbled across a good source for learning now to work with Linux, and I'm quite happy to share it with you. Here you go!

	Linux Tutorial: http://www.ee.surrey.ac.uk/Teaching/Unix/



***************************
*** Folder Descriptions ***
***************************

Below is a description of each folder and its contents.


 * functions *
	Here lies many of the functions and classes used in the main Quadrotor program. The
	functions here are kept separate mostly for the sake of cleanliness and to prevent
	clutter. 
	
	- "autopilot": contains functions for communicating with the High Level Processor 
		(HLP) found on the AscTec Hummingbird's Autopilot after custom code has been 
		installed. See my thesis for a detailed breakdown of the data frames used to
		communicate with the HLP.
	- "Controller": A class for performing control laws. Feel free to add functions for
		new control laws, then call them in the appropriate places!
	- "feedbackLin": contains functions necessary for performing the feedback
		linearization process discussed in my thesis. It's quite dense. See my thesis
		for more information.
	- "Filter3": The third attempt at implementing filtering. Contains functions for
		performing the Quick Filter discussed in my thesis, as well as for trying 
		(unsuccessfully) to implement an Extended Kalman Filter using OpenCV. The EKF
		refused to run fast enough. 
	- "Timer": A small class used for tracking time differences like a stopwatch. 
	- "TrajectoryMaker": much like the "Controller" class, this class was used for
		generating trajectories based on various flight modes. A lot of improvement can
		be done here.
	- "transforms": Several functions used for various coordinate system transforms.


 * gcs *

	The folder containing all of the relevant files for the Qt-based Ground Control
	Station (GCS) program. This program was fairly hacked together, so someone with more
	Qt knowledge than me can likely redo this and make it a lot better. But, it works.
	See the "SETUP PROCESS" section to learn how to install Qt and Qt Creator, and open
	"gcs/gcs/gcs.pro" in Qt Creator to start working on it.


 * lib *
	Here are the necessary Vicon libraries for contacting the Vicon server, as well as
	the GNU Linear Programming Kit (GLPK) libraries for linear programming 
	optimization algorithms. See the "SETUP PROCESS" section for information related to
	the library's installation.

	- "libViconDataStreamSDK_CPP_32/64.so": The 32/64-bit Vicon DataStream SDK libraries
		used for obtaining information from the Vicon server. These are simply
		renamed versions of the Datastream SDK v1.2 files.
	- "glpk-4.55": contains the source code for the GLPK 


 * opt *
	Contains functions that implement linear programming optimization problems. See my 
	thesis for information on what this is trying to achieve. 
	 
	- "YawOptimizer": A class designed to find the minimum/maximum yaw torque possible
		while still maintaining the same thrust and pitch/roll torques.
	- "PitchRollLimiter": A class designed to scale back a set of pitch/roll torques
		while preserving their ratio while maintaining a desired thrust. The yaw torque
		is left as a free parameter.
	- "FeasibilitySearch": multiple functions used to test the timing associated with the
		various optimization methods. 


 * quad *
	Contains the main function designed to run on the quadrotor, the main Quadrotor
	class, information/functions related to discrete flight modes, and the parameters
	for different quadrotors.

	- "mainProgram": The main program that gets compiled and run on the quadrotor's 
		Intel Edison. 
	- "modes": performs various function calls based on the discrete flight mode of the
		quadrotor. See "Mode Documentation" for more information as to what each mode
		does, and see the "Mode Transition Limitaiton Rules" Excel spreadsheet for a 
		layout of the permittable transitions based on the GCS's desired flight mode.
	- "parameters": the main file used to enter the parameters (mass, rotational inertia,
		propeller coefficients, etc.) for a given quadrotor. Each index is associated
		with a particular quadrotor; by changing the index used to identify a given
		quadrotor, 
	- "Quadrotor": the main quadrotor class used by "mainProgram". Calls the appropriate
		functions in each of the subclasses for performing all of its operations.
	

 * socket *
	Contains classes and functions related to passing information over IP sockets.

	- "Beej_Guide_to_Network_Programming.pdf": a good guide for working with IP socketing
		in C and C++. 
	- "packet": functions and structs associated with packing information into/
		extracting information out of packets/frames for transmission over IP sockets.
	- "QuadClient": a class designed to operate as an IP socket client that connects to
		a server (QuadServer, to be specific).
	- "QuadServer": a class designed to operate as an IP socket server that receives a
		connection from a single client (QuadClient).
	- "test" files: Files that can be used to test the functionality of the clients,
		servers, and packets.


 * thread *
	Contains classes used to perform multithreaded operations, as well as test functions
	that can be used to verify their performance.

	- "POSIX Threads Programming Resource": a good resource for learning how to use the 
		POSIX libraries for performing C/C++ multithreading in Linux. 
	- "AutopilotThread": a class that configures the serial port to talk to the 
		Autopilot's HLP, then dedicates a thread to reading incoming information from the
		HLP. Function calls can be used to send information to the HLP.
	- "GcsServerThread": a multithreaded class designed to be integrated into the GCS for
		communicating back and forth with the quadrotor's client class
		(QuadClientThread).
	- "LoggerThread": a class that dedicates a thread to making file write operations
		to record data. Data is saved in a Comma Separated Value (CSV) format. This
		prevents file write operations from holding up the main process.
	- "QuadClientThread": a multithreaded class designed to be integrated into the
		Quadrotor class for communicating back and forth with the GCS's server class 
		(GcsServerThread).
	- "ViconThread": a class that dedicates a thread to reading incoming Vicon
		measurements.
	- "example" files: files for testing compilation and functionality of the above 
		classes.


 * vicon *
	Contains a class designed to make use of the Vicon libraries to talk to the Vicon
	motion capture system server. 

	- "Client.h": The header file designed to work with the Vicon libraries.
	- "exampleViconClass": an example function that can be used to demonstrate and/or 
		test the Vicon system's functionality.
	- "ViconClass.h": the Vicon class.
	- "Vicon Datastream SDK Manual.pdf": a PDF providing information about the available
		function calls in the Vicon library.



*****************
*** Makefiles ***
*****************

In pretty much every folder is a file called "makefile". If you're new to working with Linux, you may be asking yourself: "What the heck is a 'makefile'?" To put it simply, a "makefile" is a file that makes it easy to compile code. For example, if you want to compile the ViconClass example ("exampleViconClass.cpp" found in the "vicon" folder), the way to compile it through the terminal (with the 2011 C++ standard, a thorough error search, and for a 64-bit system) would be to type out the following:

	g++ -Wall -std=c++11 exampleViconClass.cpp ViconClass.cpp -L../lib -lViconDataStreamSDK_CPP_64 -o exampleViconClass.exe

Having to do this each and every time would get incredibly tedious. To avoid this, the makefile in "vicon" has a special set of instructions for effectively spelling out this long string of text. This allows the user to type in the following to achieve the same result:
	
	make example

The compiled file, any required objects, and any temporary files (marked with a "~" at the end) can all be cleared by using the following command:

	make clean

Take a look at the "vicon" folder's makefile to get a feel for how it does it. If it's not immediately obvious, consult the oracle (Google). To see what various things can be compiled in each folder, look at each folder's makefile for the various commands.



*********************
*** SETUP PROCESS ***
*********************

To set up your system for running the code, perform the following:

  - Get acquainted with Linux. I cannot stress this enough. If this is your first time
	using Linux, or if you're quite rusty, feel free to look at this tutorial found here.
	It won't teach you everything, but it teaches you the basics. For everything else,
	consult the oracle (Google).
	
	http://www.ee.surrey.ac.uk/Teaching/Unix/

  - Make sure you have an internet connection, which (if you were able to download this
	repository) shoudn't be an issue.

  - See the setup file for the relevant operating system on which the code is to be 
	installed ("ubuntuSetup" for a desktop/laptop, "ubilinuxSetup" for an Intel Edison
	running Ubilinux). These files provide information about the libraries to be
	installed, how to install them, and all that fun stuff. Assuming I programmed them
	right (I haven't had a good chance to test them yet), you should be able to just
	run them and they will perform everything for you. If there isn't a file for your
	particular operating system, these files can still be useful for determining what
	needs to be set up.

  - Pay special attention to the installation of the GNU Linear Programming Kit (GLPK)
	library included in the "lib/glpk-4.55" folder. After installing the library on your
	machine, re-checkout the folder ("git checkout -- glpk-4.55"). If someone ends up
	committing this folder after installing the library, it will need to be re-downloaded
	from the following link.

	GLPK Link: http://ftp.gnu.org/gnu/glpk/

  - If you want to run the ground control station (GCS) program I developed, you'll need 	 to install Qt (basically libraries for making computer applications) and Qt Creator 
	(a good IDE for developing Qt applications). The versions I used are Qt 5.4.1 and
	Qt Creator 3.3.1. Later versions may work, but I make no guarantees. The GCS program
	uses some Linux dependencies in some of the C++ code I used for it (for IP 
	socketing), though with some knowhow and/or determination, it can be made 
	cross-platform to work on whatever OS you want. 

