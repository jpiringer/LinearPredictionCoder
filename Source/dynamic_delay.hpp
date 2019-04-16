//
//  dynamic_delay.hpp
//  LinearPredictionCoder
//
//  Created by joerg piringer on 16.04.19.
//  Copyright © 2019 jpiringer. All rights reserved.
//

#ifndef dynamic_delay_hpp
#define dynamic_delay_hpp

#include <stdio.h>
extern "C" {
#include "soundpipe.h"
}

typedef struct {
    SPFLOAT time;
    SPFLOAT feedback;
    SPFLOAT last;
    sp_auxdata buf;
    uint32_t maxbufsize;
    uint32_t bufsize;
    uint32_t bufpos;
} sp_dynamic_delay;

int sp_dynamic_delay_create(sp_dynamic_delay **p);
int sp_dynamic_delay_destroy(sp_dynamic_delay **p);
int sp_dynamic_delay_init(sp_data *sp, sp_dynamic_delay *p, SPFLOAT maxtime);
int sp_dynamic_delay_compute(sp_data *sp, sp_dynamic_delay *p, SPFLOAT *in, SPFLOAT *out);
int sp_dynamic_delay_set_time(sp_data *sp, sp_dynamic_delay *p, SPFLOAT time);

#endif /* dynamic_delay_hpp */
