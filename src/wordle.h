#ifndef _WORDLE_H_
#define _WORDLE_H_

#include <iostream>
#include "wordle_types.h"

void initWordle();

data_packet_t threatMessage(data_packet_t received_data);

#endif /* _WORDLE_H_ */