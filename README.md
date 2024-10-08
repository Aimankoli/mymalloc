# mymalloc
how to solve merge conflicts:
https://github.blog/developer-skills/github-education/beginners-guide-to-github-merging-a-pull-request/

Names: Aiman Koli, Marcus Bryant
netID: mk2177      mab822

Both the metadata and the payload have to be aligned with addresses that are multiples of 8. When using malloc to check for an unallocated chunk large enough to store the data, round the provided payload size to the next multiple of 8 and use that to check. Each chunk’s metadata will have a fixed size of 24 bytes, storing the allocation status of the chunk, the amount of space dedicated to the chunk’s payload, and a pointer that points to the next chunk. 

Iteration is necessary to find the “closest” unallocate chunk. When an unallocated chunk is found, check and see if the size of the payload is greater than or equal to the amount of space we want to allocate. If the chunk’s payload is less than the requested amount of space, continue iterating through the chunks until a large enough chunk is found or the end of the heap is reached. If a large enough chunk isn’t found, then return null.  If the chunk’s payload is equal to the requested amount of payload space, set the metadata’s size variable to “allocated” and return a pointer to the beginning of the chunk’s payload. If the chunk’s payload is larger than the requested space, store the size of the original chunk and split the chunk into two- first create the allocated part, then subtract the size of the allocated part from the original size and create the unallocated part. Then return a pointer to the beginning of the allocated chunk’s payload.

If the client wants to deallocate memory, we first have to find the chunk that contains the address provided by the client. To do this, we subtract 8 bytes from the provided address which takes us back to the metadata of that chunk.  After deallocating the chunk, we iterate through the entire heap from beginning to end to find any adjacent chunks that need to be coalesced.
If we find two chunks that are next to each other and are also unallocated, we combine the two by getting the entire size of the second chunk and adding it to the first chunk’s size variable.  
After merging the two chunks, we then check if the next chunk is also unallocated. If it isn’t, we exit free(), and if it is, we merge the two chunks before exiting the function call. We do this just in case we had originally deallocated a chunk that was surrounded by unallocated chunks. If handled correctly, we can make sure that there are no consecutive unallocated chunks in memory whenever we call malloc().

Important errors to look out for are calling free() with an address not provided by malloc(), calling free() with an address that’s not at the start of a chunk, and calling free() for the second time on the same pointer. We also need to make sure that we exit out of the program if a null pointer is provided by the client. We can check if the pointer points to an address provided by malloc by comparing the address it points to to the address at the beginning of the heap and the end of the heap. If it isn’t between the two, then it is a pointer that wasn’t provided by malloc. To check if the pointer points to the beginning of a chunk’s payload, we can just check if the address the pointer is pointing to is aligned with a multiple of 8. For the third case, we just have to check and see if the chunk the pointer points to is unallocated.All these checks are to be performed at the beginning of free().

To test for leak detection, we iterate through the heap and record any chunk that is still allocated. We will keep track of the number of leaked chunks and the total amount of bytes that they take up. A pointer to this function will be passed into the call for atexit() as an argument. This will occur in our initialization function. The initialization function is called at the beginning of malloc() and free() to check if the heap has been initialized yet.

For general testing, we completely allocated the heap with chunks of the same size and created a byte pattern for every chunk. We deallocated and then reallocated
the chunks in an alternating pattern and filled them with a new byte pattern. This test was to make sure that even after deallocating a chunk and replacing its
values with new ones we could be sure that the other chunks would remain untouched. For the next test, we tried to see what would happen if the client asked for
memory the size of the heap. The third tests checks if it is possible to allocate memory and then try to deallocate it twice. The fourth test sees if you can
attempt to free an address that is not at the beginning of the payload, and the last check sees if it is possible to free an address that it located well 
outside of the bounds of the heap.


