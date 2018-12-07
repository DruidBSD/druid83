To create a bootable USB device:

   Before plugging in your USB device:
   1. Execute: ls -ltr /dev/disk*
   2. Take note of the available devices

   After plugging in your USB device:
   1. [Re-]Execute: ls -ltr /dev/disk*
   2. Take note of the newly-available `disk#' device (e.g., `disk1')

   To write the ISO file to your USB device, execute:

	sudo ./iso2usb.sh ../path/to/file.iso disk#

   Taking care to replace `disk#' with the proper device-name.

To create a bootable CD/DVD:

   Insert a blank optical disc into your burner. The Mac OS should display a
   dialog with the text "You inserted a blank [CD]. Choose an action from the
   pop-up menu or click Ignore."

   From the "Action:" pop-up menu select the "Open Disk Utility" option and
   click the "OK" button (or press ENTER on the keyboard).

   From the "Disk Utility" menu bar, select "Images" then "Burn..." to bring-
   up the "Select Image to Burn" dialog box.

   Navigate to the directory containing the ISO file, select said file, and
   click "Burn" (or press ENTER on the keyboard).

   The "Burn Disc In" modal-dialog will appear, allowing you to select the
   desired drive, speed, and options. Click "Burn" when ready to proceed.
