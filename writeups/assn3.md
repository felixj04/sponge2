Assignment 3 Writeup
=============

My name: Jung Taewon 

My POVIS ID: taewon0727

My student ID (numeric): 20220055

This assignment took me about 14 hours to do (including the time on studying, designing, and writing the code).

If you used any part of best-submission codes, specify all the best-submission numbers that you used (e.g., 1, 2): 0, 1, 2

- **Caution**: If you have no idea about above best-submission item, please refer the Assignment PDF for detailed description.

Program Structure and Design of the TCPSender:
I declared extra variables in the private member. 
is_syn, if_fin, is_timer is the variables checking each condition.
flight is the variable checks how much bytes carrying in flight.
window_size checks the size of receiver's window. 
free_space checks the size of receiver's free space. 
time_el checks elapsed time. segments{} is the queue
which finds segments that is not responsed yet. 
In the fill_window function. We first check if the connection succeeded.
We check it with SYN segment. If not, it waits for the next call.
If the window size is not 0, it sends the segment in the available 
size of window. 
If the window size is 0, it sends segment and wait. 
In the else condition, if there is no segments, it doesn't do anything.
In the ack_received function, it checks if the ACK number is available. 
If the ACK number is available, it removes the segments we recieved by ACK
in the queue. And resets the timer of transmission. After that it checks if 
any segments received the connection. It tries re-transmistion. And starts
the timer. After all things, this function calls fill_window. 
In the tick function manages the timer. If timer isn't running, it ends 
the function. If the function is running, it puts the time in time_el. 
If the event occured time_et got resets. 
After that event occurs it calls fill_window function. 


Implementation Challenges:
Since I use the best code, I need to find out which variable 
holds the operation. 
And, I think I'm not familiar with objected oriented programming
I need to find out which class and functions do that operation. 


Remaining Bugs:
Sending connect and sending transmit had faild. 
Also I think there is a problem in window_size function. 
It cannot actually send exact size. 
I think it can't send ack with problem well. 


- Optional: I had unexpected difficulty with: Fetching the git file. I cannot fetch the git well because of some reasons that I cannot know. 

- Optional: I think you could make this assignment better by: [describe]

- Optional: I was surprised by: [describe]

- Optional: I'm not sure about: [describe]
