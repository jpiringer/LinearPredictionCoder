//
//  dynamic_delay.cpp
//  LinearPredictionCoder
//
//  Created by joerg piringer on 16.04.19.
//  Copyright © 2019 jpiringer. All rights reserved.
//

#include "dynamic_delay.hpp"

#include <stdlib.h>
#include <math.h>
#include "soundpipe.h"

int sp_dynamic_delay_create(sp_dynamic_delay **p)
{
    *p = new sp_dynamic_delay;
    return SP_OK;
}

int sp_dynamic_delay_destroy(sp_dynamic_delay **p)
{
    sp_dynamic_delay *pp = *p;
    sp_auxdata_free(&pp->buf);
    free(*p);
    return SP_OK;
}

int sp_dynamic_delay_init(sp_data *sp, sp_dynamic_delay *p, SPFLOAT time)
{
    p->time = time;
    p->bufsize = p->maxbufsize = round(time * sp->sr);
    sp_auxdata_alloc(&p->buf, p->maxbufsize * sizeof(SPFLOAT));
    p->bufpos = 0;
    p->feedback = 0;
    p->last = 0;
    return SP_OK;
}

int sp_dynamic_delay_set_time(sp_data *sp, sp_dynamic_delay *p, SPFLOAT time) {
    SPFLOAT newbufsize = round(time * sp->sr);
    
    if (newbufsize > p->maxbufsize) {
        return SP_NOT_OK;
    }
    else {
        p->bufsize = newbufsize;
        p->bufpos = p->bufpos % p->bufsize;
        return SP_OK;
    }
}

int sp_dynamic_delay_compute(sp_data *sp, sp_dynamic_delay *p, SPFLOAT *in, SPFLOAT *out)
{
    SPFLOAT delay = 0, sig = 0;
    SPFLOAT *buf = (SPFLOAT *)p->buf.ptr;
    delay = buf[p->bufpos];
    sig = (delay * p->feedback) + *in;
    buf[p->bufpos] = sig;
    p->bufpos = (p->bufpos + 1) % p->bufsize;
    *out = delay;
    return SP_OK;
}
