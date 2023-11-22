# HowTo: Install Environment for TSM_EmbHardw Laboratories

----
## Process
we need to 
1. Create an image, install it on the Pi and be able to log into it
2. Install the necessary software packages

----
## Create image
### Required:

1. Raspberry Pi Model 1 B

2. SD-Card

3. Power Supply

4. Ethernet cable

### Download RPi-Imager
Download the RPi-Imager for your machine from the [Raspberry Pi Website](https://www.raspberrypi.com/software/)

### Flash your Image onto the SD Card
Insert the SD-Card into your machine and start the RPi-Imager.

Choose the 32-bit RaspberryPi OS.

Choose your SC-Card in the imager.

Click on the gear to access the advanced settings.

1. You may change your hostname (default raspberrypi).

2. Enable SSH and set a username and password for your SSH access (You should have password authentication on).

Save the changes and write your SD-Card.

### Connecting to your Pi
Insert the SD-Card into your Pi, connect the Pi to your router and plug in the power supply. 
Let it boot up (This may take a few seconds).

Once the red LED has reached a steady state and the ethernet socket LED has started blinking, 
open a terminal and/or a command line interface.
You can get the IP address of the Pi using nslookup and the Pi's hostname and connect to it with 

```bash
ssh username@IP 
```
----
## Installing the Required Software
We need to ensure that all the required software is installed.
1. Check gcc toolchain
2. Install samba for network drive mapping (optional but recommended)
3. Install Python 3.10 or higher

### C-Compiler
Check the installation of the gcc toolchain
```bash
gcc --version
grpof --version
```
### Samba
Installing Samba allows you to access the drive of the Pi via MS Windows and hence use the IDE on your laptop.
#### Install the samba server
the necessary steps are
1. Install samba
2. Edit the configuration to make a drive/directory visible to a user(name)
3. Add the username/password to the samba user management
4. Restart the daemon so the changes can take effect
5. Attach the drive/directory as a network drive to your host (labtop) 

##### Install samba
```bash 
sudo apt-get install samba
```
##### Edit the configuration file
Edit the configuration file of the samba server in the “share definitions” section as follows 
```bash 
sudo nano /etc/samba/smb.conf
```
Change the following lnes:

~ line 189: **browseable = yes**

~ line 193: **read only = no**
##### Add the username/password to the samba user management
Add the user pi to samba's user management:
```bash
sudo smbpasswd -a pi
```
set the password to: **raspberry**
##### Restart the Daemons

```bash 
sudo service smbd restart
sudo service nmbd restart
```
### Python
We can install python using:

```bash
sudo apt update
sudo apt install python3.11
sudo apt install python3-opencv
```
Check the installation
```bash
python --version
```
-----
## Versioning