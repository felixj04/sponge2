Assignment 4 Writeup
=============

My name: [Jung Taewon]

My POVIS ID: [taewon0727]

My student ID (numeric): [20220055]

This assignment took me about [11] hours to do (including the time on studying, designing, and writing the code).

If you used any part of best-submission codes, specify all the best-submission numbers that you used (e.g., 1, 2): [0,1, 2, 3]

- **Caution**: If you have no idea about above best-submission item, please refer the Assignment PDF for detailed description.

Your benchmark results (without reordering, with reordering): [0.00, 0.00]

Program Structure and Design of the TCPConnection:
I've declared such variables as private members in
the header file. 
is_active is the variable which checks if the TCP
connection is active or inactive
time_cnt is the variable which traces the time after 
getting the segment. It is used to find if timeout
had exceeded.
setting_ack_window is a function that gets the 
information of window size and ACK. 
Remaining_outbound_capacity function returns 
the remaining outbound capacity in sender.
Bytes_in_flight function returns the bytes in flight in sender.
unassesmbled_bytes function returns unassembled bytes in receiver,
time_since_last_segment_received returns the timer
In the segment_received function, first check if RST flag has set.
If set, it is an error. If _receiver.unassembled_bytes()..., it checks 
if the segmet length is longer than 0, if the segment has a data, it 
fills the window and send the segment. 
if seg.length_... means if the segment contains the data, it fills the 
window and create segment and send it.
In the tick function, I made two variables which checks the state of 
input and output. And update the time_cnt variable. 
If segment queue is not empty, it adds to the segment out queue.

Implementation Challenges:
This assigment has a lot of number of cases. Therefore, 
it was hard to make such cases. Making a lot of 
conditional states was very confusing for me.

Remaining Bugs:
A lot of test cases had failed. From case 41 it has failed.
Therefore, after case 41 it has failed.

- Optional: I had unexpected difficulty with: [describe]

- Optional: I think you could make this assignment better by: [describe]

- Optional: I was surprised by: [describe]

- Optional: I'm not sure about: [describe]
