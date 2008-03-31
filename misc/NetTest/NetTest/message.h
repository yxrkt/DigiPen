//-----------------------------------------------------------------------------
#ifndef MESSAGE_H
#define MESSAGE_H
//-----------------------------------------------------------------------------


template <typename T>
class Message
{
  public:
    Message( bool _reliable = false, bool _toResend = false )
      : reliable( _reliable ), toResend( _toResend ) {}
    void Send() {}

    bool reliable;
    bool toResend;
};


#endif
