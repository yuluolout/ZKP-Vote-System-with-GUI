# ZKP-Vote-System-with-GUI
This is our Resaerch Project product, the ZKP vote system with GUI </br>
This project is based on libsnark and fltk, please make sure you download them before use it! </br>

Project Envrionment: Ubuntu 20.04 </br>

How to use this vote system?</br>
First, install the fltk by command: sudo apt install libfltk1.3-dev </br>
Second, install the libsnark library: https://github.com/scipr-lab/libsnark Please following the its README to install. </br>
Thrid, after install install the libsnark, you will have a directory called libsnark, go to /libsnark/libsnark/, then copy this project in it. </br>
After those, you can build the full environment by command cmake .. and then make in the directory /libsnark/build/ </br>
To run the whole project, run the command ./window in directory /libsnark/build/libsnark/ </br>
To run the process program, run the command ./main_vote in directory /libsnark/build/libsnark/ </br>
If you failed with user login or registration, please check if you have "user_list.txt" or "voting_record.txt" </br>

Hope you enjoy this simple ZKP vote system!
