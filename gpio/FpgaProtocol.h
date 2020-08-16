#ifndef FPGAPROTOCOL_H
#define FPGAPROTOCOL_H

struct FpgaProtocol;

typedef struct FpgaProtocol FpgaProtocol;

void FpgaProtocol__init(FpgaProtocol* self);

FpgaProtocol* FpgaProtocol__create();

void FpgaProtocol__setMovementCallback(FpgaProtocol* self, void (*callback)(float, float, float));

void FpgaProtocol__setColourCallback(FpgaProtocol* self, void (*callback)(int, int, int));

void FpgaProtocol__setButtonCallback(FpgaProtocol* self, void (*callback)(int*));

#endif
