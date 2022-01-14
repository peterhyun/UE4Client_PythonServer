# UE4Client_PythonServer
I connected my UE4 project(Engine Ver. 4.21) to my TCP server I created via Python

The TCP server reads the test_anim.npy file and gets the animation data of the agent.
Then it sends this data to the UE4 TCP Client and the TCP Client renders the data appropriately.

Here is a demo below:
https://www.youtube.com/watch?v=w9-5oTsfjoY

Make sure to configure the custom c++ class appropriately in the UE4 blueprint settings after importing the c++ code!
