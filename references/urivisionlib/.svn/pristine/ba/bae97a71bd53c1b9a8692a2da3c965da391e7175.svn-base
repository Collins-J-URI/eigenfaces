#include "BufferRing.h"

using namespace uriVL;

/*
 *	Initializes the BufferRing to the specified 
 *	number of buffers.
 */
BufferRing::BufferRing(int ringSize)
{
	ringSize_ = ringSize;
	locked_ = new bool[ringSize_];

    read_ = 0;
    curr_ = 0;
    write_ = 0;
}

/*
 *	Destructor calls release() method to handle clean-up
 */
BufferRing::~BufferRing()
{
	// release que
	if (ring_ != NULL)
    	delete[] ring_;

    if (locked_ != NULL)	
    	delete[] locked_;

}

//locks BufferRing at read_ position
void BufferRing::lock(void)
{
	recentLock_ = read_;
	locked_[recentLock_] = true;
}

//locks BufferRing at index
void BufferRing::lock(int index)
{
	recentLock_ = index;
	locked_[recentLock_] = true;
}

//unlocks BufferRing at index
void BufferRing::unlock(int index)
{
	locked_[index] = false;
}

//determines whether or not index of BufferRing is locked
bool BufferRing::isLocked(int index)
{
	return locked_[index];
}

bool BufferRing::cycle()
{
	bool fail = true;
	//this shouldn't happen but if it does...
	FAIL_CONDITION( write_ > ringSize_,
	                kVideoSourceError,
	                "write_ ended up out of bounds");

	read_ = write_;	//m_curr becomes the new read_
	if( write_ + 1 < ringSize_ )
	{
		write_ += 1;
		fail = false;
	} else {
		for( int i = 0; i < ringSize_; i++ )
		{
			if( !isLocked(i) )
			{
				if( i == read_ )
				{
					continue;
				} else {
					write_ = i;
					fail = false;
					break;
				}
			} else {
				continue;
			}
		}
	}
	if( fail )
	{
		return false;
	}
	return true;
}

bool BufferRing::write(RasterImage* image)
{
	FAIL_CONDITION( (write_ < 0) || (write_ > ringSize_),
                    kVideoSourceError,
                    "Something went wrong with the write function");

	//m_ring[write_] = &image;
	if( cycle() )
		return true;

	return false;
}

RasterImage* BufferRing::read()
{
	return &ring_[read_];
}
/*
long BufferRing::getlong()
{
	lock();	//lock the current location for reading
	return m_ring[read_];
}*/

int BufferRing::recentLock()
{
	return recentLock_;
}

//-----------------
//testing functions
//-----------------
int BufferRing::getSize()
{
	return ringSize_;
}

int BufferRing::getWriteIndex()
{
	return write_;
}

int BufferRing::getReadIndex()
{
	return read_;
}