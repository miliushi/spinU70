#ifndef P2SHUNT_H
#define  P2SHUNT_H
float *p2shunt(double p, int Version);
float shunt2p(float *shunt, int Version);
float *AllShunts2Mom(float *shunt, int Version);
#endif
