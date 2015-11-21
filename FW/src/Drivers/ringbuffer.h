#ifndef _RINGBUFFER_H_
#define _RINGBUFFER_H_

#include "Type.h"
#define k_inline                   static inline		//for IAR
//#define k_inline                   static __inline		//for ARM CC
//#define k_inline                   static __inline		//for GNU CC


typedef		uint32_t		size_t;

/* ring buffer */
struct ringbuffer
{
    uint8_t *buffer_ptr;
    /* use the msb of the {read,write}_index as mirror bit. You can see this as
     * if the buffer adds a virtual mirror and the pointers point either to the
     * normal or to the mirrored buffer. If the write_index has the same value
     * with the read_index, but in a different mirror, the buffer is full.
     * While if the write_index and the read_index are the same and within the
     * same mirror, the buffer is empty. The ASCII art of the ringbuffer is:
     *
     *          mirror = 0                    mirror = 1
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Full
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     *  read_idx-^                   write_idx-^
     *
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * | 0 | 1 | 2 | 3 | 4 | 5 | 6 ||| 0 | 1 | 2 | 3 | 4 | 5 | 6 | Empty
     * +---+---+---+---+---+---+---+|+~~~+~~~+~~~+~~~+~~~+~~~+~~~+
     * read_idx-^ ^-write_idx
     *
     * The tradeoff is we could only use 32KiB of buffer for 16 bit of index.
     * But it should be enough for most of the cases.
     *
     * Ref: http://en.wikipedia.org/wiki/Circular_buffer#Mirroring */
    uint32_t read_mirror : 1;
    uint32_t read_index : 31;
    uint32_t write_mirror : 1;
    uint32_t write_index : 31;
    /* as we use msb of index as mirror bit, the size should be signed and
     * could only be positive. */
    int32_t buffer_size;
};

enum ringbuffer_state
{
	RINGBUFFER_EMPTY,
	RINGBUFFER_FULL,
	/* half full is neither full nor empty */
	RINGBUFFER_HALFFULL,
};

k_inline uint32_t ringbuffer_get_size(struct ringbuffer *rb)
{
	ASSERT(rb != 0);
	return rb->buffer_size;
}

k_inline enum ringbuffer_state
	ringbuffer_status(struct ringbuffer *rb)
{
	if (rb->read_index == rb->write_index)
	{
		if (rb->read_mirror == rb->write_mirror)
			return RINGBUFFER_EMPTY;
		else
			return RINGBUFFER_FULL;
	}
	return RINGBUFFER_HALFFULL;
}

/** return the size of data in rb */
k_inline uint32_t ringbuffer_data_len(struct ringbuffer *rb)
{
	switch (ringbuffer_status(rb))
	{
	case RINGBUFFER_EMPTY:
		return 0;
	case RINGBUFFER_FULL:
		return rb->buffer_size;
	case RINGBUFFER_HALFFULL:
	default:
		if (rb->write_index > rb->read_index)
			return rb->write_index - rb->read_index;
		else
			return rb->buffer_size - (rb->read_index - rb->write_index);
	};
}

/** return the size of empty space in rb */
#define ringbuffer_space_len(rb) ((rb)->buffer_size - ringbuffer_data_len(rb))


void ringbuffer_init(struct ringbuffer *rb,
	uint8_t           *pool,
	int32_t            size);
size_t ringbuffer_put(struct ringbuffer *rb,
	const uint8_t     *ptr,
	uint32_t           length);
size_t ringbuffer_put_force(struct ringbuffer *rb,
	const uint8_t     *ptr,
	uint32_t           length);
size_t ringbuffer_putchar(struct ringbuffer *rb,
	const uint8_t      ch);
size_t ringbuffer_putchar_force(struct ringbuffer *rb,
	const uint8_t      ch);
size_t ringbuffer_get(struct ringbuffer *rb,
	uint8_t           *ptr,
	uint32_t           length);
size_t ringbuffer_getchar(struct ringbuffer *rb, uint8_t *ch);

void ringbuffer_reset(struct ringbuffer *rb);

#endif