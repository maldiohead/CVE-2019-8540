# CVE-2019-8540
Kernel Stack info leak at exportObjectToClient function

 bug details:
 
 macOS<=10.14.3 && iOS < 12.2
 
 
There is a bug at Function exportObjectToClient  in  IOKit class, which can lead to  leak 4 bytes of kernel stack info.
exportObjectToClient  just like its name which make an arbitrary OSObject available to the client task.It’s a basic  function, many other kernel  function use it .

   
`IOReturn IOUserClient::exportObjectToClient(task_t task, OSObject *obj, io_object_t *clientObj)`

`{`
    `mach_port_name_t name;    
    name = IOMachPort::makeSendRightForTask( task, obj, IKOT_IOKIT_OBJECT );`
    `*(mach_port_name_t *)clientObj = name; // (1). force the type convert to mach_port_name_t` 
    `if (obj) obj->release();`
    `return kIOReturnSuccess;`
`}`

We know io_object_t length is 8 bytes  and at (1) which  was  force convert to mach_port_name_t(4 bytes) ,so lead to  the high 4 bytes not inital
