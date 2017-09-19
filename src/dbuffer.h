#pragma once
#include "std_types.h"

typedef struct dbuffer {
    u8* start;
    u8* end;  // [start; end[
    u8* head;
} dbuffer;

void dbuffer_init_with_capacity(dbuffer* db, ureg size);
void dbuffer_init(dbuffer* db);
void dbuffer_free(dbuffer* db);

ureg dbuffer_get_capacity(dbuffer* db);
ureg dbuffer_get_size(dbuffer* db);

void dbuffer_get_from_idx(dbuffer* db, void* target, ureg pos, ureg size);
void dbuffer_get(dbuffer* db, void* target, void* pos, ureg size);

bool dbuffer_is_emtpy(dbuffer* db);
void dbuffer_set_capacity(dbuffer* db, ureg new_size);
void dbuffer_set_bigger_capacity(dbuffer* db, ureg new_size);
void dbuffer_grow(dbuffer* db);
int dbuffer_has_space(dbuffer* db, ureg required_space);
void dbuffer_make_space(dbuffer* db, ureg required_space);
void dbuffer_make_small_space(dbuffer* db, ureg required_space);

void* dbuffer_claim_space(dbuffer* db, ureg required_space);
void* dbuffer_claim_small_space(dbuffer* db, ureg required_space);

void dbuffer_remove_at(dbuffer* db, void* pos, ureg size);
void dbuffer_insert_at(dbuffer* db, const void* data, void* pos, ureg size);
void dbuffer_swap(dbuffer* db, void* posa, void* posb, ureg size);

void dbuffer_pop_back(dbuffer* db, ureg size);

// all push backs
void dbuffer_push_back(dbuffer* db, const void* data, ureg size);

void dbuffer_push_back_u8(dbuffer* db, u8 d);
void dbuffer_push_back_s8(dbuffer* db, s8 d);

void dbuffer_push_back_u16(dbuffer* db, u16 d);
void dbuffer_push_back_s16(dbuffer* db, s16 d);

void dbuffer_push_back_u32(dbuffer* db, u32 d);
void dbuffer_push_back_s32(dbuffer* db, s32 d);
void dbuffer_push_back_f32(dbuffer* db, f32 d);

void dbuffer_push_back_u64(dbuffer* db, u64 d);
void dbuffer_push_back_s64(dbuffer* db, s64 d);
void dbuffer_push_back_f64(dbuffer* db, f64 d);

void dbuffer_push_back_ureg(dbuffer* db, ureg d);
void dbuffer_push_back_sreg(dbuffer* db, sreg d);
void dbuffer_push_back_uregh(dbuffer* db, uregh d);
void dbuffer_push_back_sregh(dbuffer* db, sregh d);
void dbuffer_push_back_uregq(dbuffer* db, uregq d);
void dbuffer_push_back_sregq(dbuffer* db, sregq d);

void dbuffer_push_back_ptr(dbuffer* db, void* d);
