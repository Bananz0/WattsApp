//
// Created by glenm on 2/2/2025.
//

#pragma once

#ifndef PORTHANDLER_H
#define PORTHANDLER_H

//Define portType as enum
#undef INPUT
#undef OUTPUT
typedef enum {
    INPUT = 0x00,
    OUTPUT = 0xFF
} PortType;

void initializePorts(const char portName, const PortType portType );

void finalizePorts();

#endif //PORTHANDLER_H
