# ipc-cs-pancake
IPC Client server pancake is project for client server communication using IPC concepts.

The Project tends to use all IPC Concepts including fork,excl,pipe,fifo,message queue,shared memory,thread, thread semaphore, signal handling for client and servers.
Client under a seprate process will work on threads and we plan to create 1Lakh threads under client process, these process will be sending request to Server process using fifo. Then fifo will check the request and process that and pass it to request specific vender using pipe. Then vender will be processing the results and sharing them back to server using shared memory, After that Server will use message queue to send the result back to client.
