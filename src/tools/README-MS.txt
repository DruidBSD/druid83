To create a bootable USB device [using the Windows GUI]:

   First, rename the `*.iso' file to `*.img' (the Win32DiskImager program will
   not allow you to open the ISO file unless it is named `*.img').

   Navigate to the "win32diskimager-binary" sub-directory and run the
   "Win32DiskImager.exe" program.

   The "Win32 Disk Imager" window is very simple, containing only:
   1. A text box for displaying the path to your ISO file
   2. A button with a folder-icon to browse for an ISO file
   3. A drop-down list of available USB devices
   4. A refresh-button for refreshing the list of USB devices
   5. A progress bar
   6. A "Read" button for reading the USB device -to- an ISO file
   7. A "Write" button for writing the ISO file -to- the USB device
   8. A "Cancel" button, and
   9. An "Exit" button.

   Click the folder-icon button to browse for the ISO file.

   Next, select the USB device you wish to write the ISO file to.

   Click the "Write" button.

   When done, click the "Exit" button.

To create a bootable USB device [using the DOS command-prompt]:

   First -- in a DOS command-prompt -- navigate to this directory.

   NOTE: On Windows Vista and Windows 7, you will need to launch the DOS
         command-prompt as the Administrator. This can be done by navigating
         to (in most-cases) C:\WINDOWS\system32\ and then right-clicking on
         `cmd.exe' and selecting "Run as administrator".

   Before plugging in your USB device:
   1. Execute: dd --list --filter=removable
   2. Take note of the available devices
   3. Output may resemble the following (showing no removable USB devices):

	C:\Users\dteske>dd --list --filter=removable
	rawwrite dd for windows version 0.6beta3.
	Written by John Newbigin <jn@it.swin.edu.au>
	This program is covered by terms of the GPL Version 2.
	
	NT Block Device Objects

   After plugging in your USB device:
   1. [Re-]Execute: dd --list --filter=removable
   2. Take note of the newly-available device
   3. Output may resemble the following (showing one removable USB device):

	C:\Users\dteske\Installers\Vicor>dd --list --filter=removable
	rawwrite dd for windows version 0.6beta3.
	Written by John Newbigin <jn@it.swin.edu.au>
	This program is covered by terms of the GPL Version 2.
	
	NT Block Device Objects
	\\?\Device\Harddisk4\Partition0
	  link to \\?\Device\Harddisk4\DR14
	  Removable media other than floppy. Block size = 512
	  size is 4040724480 bytes

   NOTE: Both Windows Vista and Windows 7 require administrative rights to
         list removable USB devices. If running either of these versions of
         Windows and -- after plugging-in your USB device -- you still cannot
         list your device, try running C:\WINDOWS\system32\cmd.exe as the
         administrator (see NOTE above).

   To write the ISO file to your USB device, execute:

	dd if=Druid-X.Y.iso of=\\?\Device\Harddisk4\Partition0 \
		bs=512k conv=sync --progress

   Taking care to replace `\\?\Device\Harddisk4\Partition0' with the proper
   device-name.

   NOTE: The above is a single-command, executed on a single-line. Do not type
         the trailing "\" at the end of the first line.

To create a bootable CD/DVD:

   Download and install the ImgBurn software, available for free from:

	http://imgburn.com/

   Once-installed, run the ImgBurn software and select the "Write image file
   to disc" option.

   Next, click the folder-icon button to browse for the ISO file.

   Once-selected, click the large button at the bottom-left of the window to
   burn the ISO file to optical media.

