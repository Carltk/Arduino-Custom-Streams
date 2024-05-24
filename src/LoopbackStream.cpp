#include <LoopbackStream.h>

LoopbackStream::LoopbackStream(uint16_t buffer_size) {
  this->buffer = (uint8_t*) malloc(buffer_size);
  this->buffer_size = buffer_size;
  this->pos = 0;
  this->size = 0;
}
LoopbackStream::~LoopbackStream() {
  free(buffer);
}

/**************************************************************************/
/*!
  @brief  Clear the Stream
  @param  none
  @return none
  */
/**************************************************************************/
void LoopbackStream::clear() {
  this->pos = 0;
  this->size = 0;
}

/**************************************************************************/
/*!
  @brief  Read a byte from the Stream
  @param  none
  @return The byte read - as an Integer
  */
/**************************************************************************/
int LoopbackStream::read() {
  if (size == 0) {
    return -1;
  } else {
    int ret = buffer[pos];
    pos++;
    size--;
    if (pos == buffer_size) {
      pos = 0;
    }
    return ret;
  }
}

/**************************************************************************/
/*!
  @brief  Write a byte TO the Stream
  @param  v The byte to write
  @return The number of bytes written (can be 0 if there is no room)
  */
/**************************************************************************/
size_t LoopbackStream::write(uint8_t v) {
  if (size == buffer_size) {
    return 0;
  } else {
    int p = pos + size;
    if (p >= buffer_size) {
      p -= buffer_size;
    }
    buffer[p] = v;
    size++;
    return 1;
  }  
}

/**************************************************************************/
/*!
  @brief  Copy contents of supplied buffer into the Stream
  @param  buf The 0-delimited buffer to copy into the Stream
  @return The number of bytes written
  */
/**************************************************************************/
int LoopbackStream::push(char* buf) {

  if (size == buffer_size) 
  { return 0;   } 
  else 
  { int i = 0;
    int p;   
    
    while (buf[i] != 0) {
      p = pos + size;                             // p=current write position, pos=current read position, size is num bytes used
      if (p >= buffer_size) (p -= buffer_size);   // roll the write position pointer if necessary
      if (size == buffer_size) return i;          // check for buffer exceeded & abort with current chars written
      buffer[p] = buf[i];                         // move the byte across
      size++;                                     // inc the num bytes used
      i++;                                        // inc pointer to the input buffer (and chars written count)
    }  
    return i;
  }  
}

/**************************************************************************/
/*!
  @brief  Number of bytes available to be read from the Stream
  @param  none
  @return The number of bytes available to be read 
*/
/**************************************************************************/
int LoopbackStream::available() {
  return size;
}

int LoopbackStream::availableForWrite() {
  return buffer_size - size;
}

bool LoopbackStream::contains(char ch) {
  for (int i=0; i<size; i++){
    int p = (pos + i) % buffer_size;
    if (buffer[p] == ch) {
      return true;
    }
  }
  return false;
}

/**************************************************************************/
/*!
  @brief  Drop the number of chars from the Stream
  @param  chars
  @return The number of characters droppped. Return -1 if out of range.
*/
/**************************************************************************/
int LoopbackStream::drop(uint8_t chars) {
  if (size == 0 ) return -1;                  // No chars in the Stream .. -1
  if (chars > size) return -1;               // required drop is past contents of the Stream .. -1
  pos = (pos + chars) % buffer_size;         // roll the read position pointer if necessary
  size -= chars;                              // dec the number of bytes used
  return chars;                               // return the number of chars dropped
}

/**************************************************************************/
/*!
  @brief  Look for a character in the Stream that matches a charcter in the supplied character array (mask)
  @param  mask A character array of bytes to find (similar to strchr)
  @param  offset Start this number of chars into the start of the Stream
  @return The number of bytes before the character found, or -1 if not found
*/
/**************************************************************************/
int LoopbackStream::findUntilChr(const char* mask, uint8_t offset) {
  
  for (int i=offset; i<size; i++){
    int p = (pos + i) % buffer_size;

    if (strchr(mask, (int)buffer[p])) 
    { return i; }
  }
  return -1; 
}

/**************************************************************************/
/*!
  @brief  Return the current character from the Stream without moving the read pointer 
  @param  none
  @return The character at the current read position
*/
/**************************************************************************/
int LoopbackStream::peek() {
  return size == 0 ? -1 : buffer[pos];
}

/**************************************************************************/
/*!
  @brief  Return the current character from the Stream at the position "offset" past the read pointer
  @param  offset
  @return The character at the required position. Return -1 if out of range
*/
/**************************************************************************/
int LoopbackStream::peekAt(uint8_t offset) {
  if (size == 0 ) return -1;                  // No chars in the Stream .. -1
  if (offset > size) return -1;               // required char is past contents of the Stream .. -1
  int p = (pos + offset);                     // get the required position
  if (p >buffer_size) (p = p - buffer_size);  // roll the read position pointer if necessary
  int ret = buffer[p];                         // return the character
  return ret;                           // return the character
}

void LoopbackStream::flush() {
  //I'm not sure what to do here...
}

