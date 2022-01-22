# UE4Client_PythonServer
I connected my UE4 project(Engine Ver. 4.21) to my TCP server I created via Python

The TCP server reads the 'test_anim.npy' file and gets the animation data of the agent.
Then it sends this data to the UE4 TCP Client and the TCP Client applies the animation data to the model in real time.

How to run)
1. Import all the code in the ```/UE4Client``` directory to your UE4.21 project. Then build the project again. Configure the AnimGraphNode_CustomControl and FAnimNode_CustomControl in the UE4 agent's Blueprint settings. (Replace it from the original nodes)

2. Go to the ```/PythonServer``` directory and run TCPServer.py via ```python TCPServer.py``` command in your terminal. This will open up the python server.

3. Play your UE4 client project you configured.

Here is a demo below:
https://www.youtube.com/watch?v=w9-5oTsfjoY

