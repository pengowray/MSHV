 /* MSHV 
 * WSJT-X MessageClient created by G4WJS 
 * and modified for MSHV
 * May be used under the terms of the GNU General Public License (GPL)
 */

#ifndef NETWORKMESSAGE_H
#define NETWORKMESSAGE_H

/*
 * WSJT-X Message Formats
 * ======================
 *
 * All messages are written or  read using the QDataStream derivatives
 * defined below, note that we are using the default for floating
 * point precision which means all are double precision i.e. 64-bit
 * IEEE format.
 *
 *  Message is big endian format
 *
 *   Header format:
 *
 *      32-bit unsigned integer magic number 0xadbccbda
 *      32-bit unsigned integer schema number
 *
 *   Payload format:
 *
 *      As per  the QDataStream format,  see below for version  used and
 *      here:
 *
 *        http://doc.qt.io/qt-5/datastreamformat.html
 *
 *      for the serialization details for each type, at the time of
 *      writing the above document is for Qt_5_0 format which is buggy
 *      so we use Qt_5_4 format, differences are:
 *
 *      QDateTime:
 *           QDate      qint64    Julian day number
 *           QTime      quint32   Milli-seconds since midnight
 *           timespec   quint8    0=local, 1=UTC, 2=Offset from UTC
 *                                                 (seconds)
 *                                3=time zone
 *           offset     qint32    only present if timespec=2
 *           timezone   several-fields only present if timespec=3
 *
 *      we will avoid using QDateTime fields with time zones for simplicity.
 *
 * Type utf8  is a  utf-8 byte  string formatted  as a  QByteArray for
 * serialization purposes  (currently a quint32 size  followed by size
 * bytes, no terminator is present or counted).
 *
 * The QDataStream format document linked above is not complete for
 * the QByteArray serialization format, it is similar to the QString
 * serialization format in that it differentiates between empty
 * strings and null strings. Empty strings have a length of zero
 * whereas null strings have a length field of 0xffffffff.
 *
 * Schema Negotiation
 * ------------------
 *
 * The NetworkMessage::Builder  class specifies a schema  number which
 * may be  incremented from time to  time. It represents a  version of
 * the underlying encoding schemes used to store data items. Since the
 * underlying  encoding  is   defined  by  the  Qt   project  in  it's
 * QDataStream  stream operators,  it  is essential  that clients  and
 * servers  of  this protocol  can  agree  on  a common  scheme.   The
 * NetworkMessage  utility classes  below exchange  the schema  number
 * actually used.  The handling of  the schema is backwards compatible
 * to  an  extent,  so  long   as  clients  and  servers  are  written
 * correctly. For  example a server  written to any  particular schema
 * version can communicate with a client written to a later schema.
 *
 * Schema Version 1:- this schema used the QDataStream::Qt_4_8 version
 *  HV
 *
 * Schema Version 2:- this schema uses the QDataStream::Qt_5_2 version.
 *
 * Schema Version 3:- this schema uses the QDataStream::Qt_5_4 version.
 *
 *
 *
 * Message       Direction Value                  Type
 * ------------- --------- ---------------------- -----------
 * Heartbeat     Out/In    0                      quint32
 *                         Id (unique key)        utf8
 *                         Maximum schema number  quint32
 *
 *    The heartbeat  message shall be  sent on a periodic  basis every
 *    NetworkMessage::pulse   seconds   (see    below),   the   WSJT-X
 *    application  does  that  using the  MessageClient  class.   This
 *    message is intended to be used by servers to detect the presence
 *    of a  client and also  the unexpected disappearance of  a client
 *    and  by clients  to learn  the schema  negotiated by  the server
 *    after it receives  the initial heartbeat message  from a client.
 *    The message_aggregator reference server does just that using the
 *    MessageServer class. Upon  initial startup a client  must send a
 *    heartbeat message as soon as  is practical, this message is used
 *    to negotiate the maximum schema  number common to the client and
 *    server. Note  that the  server may  not be  able to  support the
 *    client's  requested maximum  schema  number, in  which case  the
 *    first  message received  from the  server will  specify a  lower
 *    schema number (never a higher one  as that is not allowed). If a
 *    server replies  with a lower  schema number then no  higher than
 *    that number shall be used for all further outgoing messages from
 *    either clients or the server itself.
 *
 *    Note: the  "Maximum schema number"  field was introduced  at the
 *    same time as schema 3, therefore servers and clients must assume
 *    schema 2 is the highest schema number supported if the Heartbeat
 *    message does not contain the "Maximum schema number" field.
 *
 *
 * Status        Out       1                      quint32
 *                         Id (unique key)        utf8
 *                         Dial Frequency (Hz)    quint64
 *                         Mode                   utf8
 *                         DX call                utf8
 *                         Report                 utf8
 *                         Tx Mode                utf8
 *                         Tx Enabled             bool
 *                         Transmitting           bool
 *                         Decoding               bool
 *                         Rx DF                  qint32
 *                         Tx DF                  qint32
 *                         DE call                utf8
 *                         DE grid                utf8
 *                         DX grid                utf8
 *                         Tx Watchdog            bool
 *
 *    WSJT-X  sends this  status message  when various  internal state
 *    changes to allow the server to  track the relevant state of each
 *    client without the need for  polling commands. The current state
 *    changes that generate status messages are:
 *
 *      Application start up,
 *      "Enable Tx" button status changes,
 *      Dial frequency changes,
 *      Changes to the "DX Call" field,
 *      Operating mode changes,
 *      Transmit mode changed (in dual JT9+JT65 mode),
 *      Changes to the "Rpt" spinner,
 *      After an old decodes replay sequence (see Replay below),
 *      When switching between Tx and Rx mode,
 *      At the start and end of decoding,
 *      When the Rx DF changes,
 *      When the Tx DF changes,
 *      When the DE call or grid changes (currently when settings are exited),
 *      When the DX call or grid changes,
 *      When the Tx watchdog is set or reset.
 *
 *
 * Decode        Out       2                      quint32
 *                         Id (unique key)        utf8
 *                         New                    bool
 *                         Time                   QTime
 *                         snr                    qint32
 *                         Delta time (S)         float (serialized as double)
 *                         Delta frequency (Hz)   quint32
 *                         Mode                   utf8
 *                         Message                utf8
 *
 *      The decode message is sent when  a new decode is completed, in
 *      this case the 'New' field is true. It is also used in response
 *      to  a "Replay"  message where  each  old decode  in the  "Band
 *      activity" window, that  has not been erased, is  sent in order
 *      as  a one  of  these  messages with  the  'New'  field set  to
 *      false. See the "Replay" message below for details of usage.
 *
 *
 * Clear         Out       3                      quint32
 *                         Id (unique key)        utf8
 *
 *      This message is  send when all prior "Decode"  messages in the
 *      "Band activity"  window have been discarded  and therefore are
 *      no long available for actioning  with a "Reply" message. It is
 *      sent when the user erases  the "Band activity" window and when
 *      WSJT-X  closes down  normally. The  server should  discard all
 *      decode messages upon receipt of this message.
 *
 *
 * Reply         In        4                      quint32
 *                         Id (target unique key) utf8
 *                         Time                   QTime
 *                         snr                    qint32
 *                         Delta time (S)         float (serialized as double)
 *                         Delta frequency (Hz)   quint32
 *                         Mode                   utf8
 *                         Message                utf8
 *
 *      In order for a server  to provide a useful cooperative service
 *      to WSJT-X it  is possible for it to initiate  a QSO by sending
 *      this message to a client. WSJT-X filters this message and only
 *      acts upon it  if the message exactly describes  a prior decode
 *      and that decode  is a CQ or QRZ message.   The action taken is
 *      exactly equivalent to the user  double clicking the message in
 *      the "Band activity" window. The  intent of this message is for
 *      servers to be able to provide an advanced look up of potential
 *      QSO partners, for example determining if they have been worked
 *      before  or if  working them  may advance  some objective  like
 *      award progress.  The  intention is not to  provide a secondary
 *      user  interface for  WSJT-X,  it is  expected  that after  QSO
 *      initiation the rest  of the QSO is carried  out manually using
 *      the normal WSJT-X user interface.
 *
 *
 * QSO Logged    Out       5                      quint32
 *                         Id (unique key)        utf8
 *                         Date & Time Off        QDateTime
 *                         DX call                utf8
 *                         DX grid                utf8
 *                         Tx frequency (Hz)      quint64
 *                         Mode                   utf8
 *                         Report send            utf8
 *                         Report received        utf8
 *                         Tx power               utf8
 *                         Comments               utf8
 *                         Name                   utf8
 *                         Date & Time On         QDateTime
 *                         Operator call          utf8
 *                         My call                utf8
 *                         My grid                utf8
 *
 *      The  QSO logged  message is  sent  to the  server(s) when  the
 *      WSJT-X user accepts the "Log  QSO" dialog by clicking the "OK"
 *      button.
 *
 *
 * Close         Out       6                      quint32
 *                         Id (unique key)        utf8
 *
 *      Close is sent by a client immediately prior to it shutting
 *      down gracefully.
 *
 *
 * Replay        In        7                      quint32
 *                         Id (unique key)        utf8
 *
 *      When a server starts it may  be useful for it to determine the
 *      state  of preexisting  clients. Sending  this message  to each
 *      client as it is discovered  will cause that client (WSJT-X) to
 *      send a "Decode" message for each decode currently in its "Band
 *      activity"  window. Each  "Decode" message  sent will  have the
 *      "New" flag set to false so that they can be distinguished from
 *      new decodes. After  all the old decodes have  been broadcast a
 *      "Status" message  is also broadcast.  If the server  wishes to
 *      determine  the  status  of  a newly  discovered  client;  this
 *      message should be used.
 *
 *
 * Halt Tx       In        8
 *                         Id (unique key)        utf8
 *                         Auto Tx Only           bool
 *
 *      The server may stop a client from transmitting messages either
 *      immediately or at  the end of the  current transmission period
 *      using this message.
 *
 *
 * Free Text     In        9
 *                         Id (unique key)        utf8
 *                         Text                   utf8
 *                         Send                   bool
 *
 *      This message  allows the server  to set the current  free text
 *      message content. Sending this  message with a non-empty "Text"
 *      field is equivalent to typing  a new message (old contents are
 *      discarded) in to  the WSJT-X free text message  field or "Tx5"
 *      field (both  are updated) and if  the "Send" flag is  set then
 *      clicking the "Now" radio button for the "Tx5" field if tab one
 *      is current or clicking the "Free  msg" radio button if tab two
 *      is current.
 *
 *      It is the responsibility of the  sender to limit the length of
 *      the  message   text  and   to  limit   it  to   legal  message
 *      characters. Despite this,  it may be difficult  for the sender
 *      to determine the maximum message length without reimplementing
 *      the complete message encoding protocol. Because of this is may
 *      be better  to allow any  reasonable message length and  to let
 *      the WSJT-X application encode and possibly truncate the actual
 *      on-air message.
 *
 *      If the  message text is  empty the  meaning of the  message is
 *      refined  to send  the  current free  text  unchanged when  the
 *      "Send" flag is set or to  clear the current free text when the
 *      "Send" flag is  unset.  Note that this API does  not include a
 *      command to  determine the  contents of  the current  free text
 *      message.
 *
 * WSPRDecode    Out       10                     quint32
 *                         Id (unique key)        utf8
 *                         New                    bool
 *                         Time                   QTime
 *                         snr                    qint32
 *                         Delta time (S)         float (serialized as double)
 *                         Frequency (Hz)         quint64
 *                         Drift (Hz)             qint32
 *                         Callsign               utf8
 *                         Grid                   utf8
 *                         Power (dBm)            qint32
 *
 *      The decode message is sent when  a new decode is completed, in
 *      this case the 'New' field is true. It is also used in response
 *      to  a "Replay"  message where  each  old decode  in the  "Band
 *      activity" window, that  has not been erased, is  sent in order
 *      as  a one  of  these  messages with  the  'New'  field set  to
 *      false. See the "Replay" message below for details of usage.
 *
 *
 */

#include <QDataStream>

#include "pimpl_h.h"

class QIODevice;
class QByteArray;
class QString;

namespace NetworkMessage
{
  // NEVER DELETE MESSAGE TYPES
  enum Type
    {
      Heartbeat,			//00
      Status,				//01
      Decode,				//02
      Clear,				//03
      Reply,				//04
      QSOLogged,			//05
      Close,				//06
      Replay,				//07
      HaltTx,				//08
      FreeText,				//09
      WSPRDecode,			//0a
      Location,				//0b
      LoggedADIF,			//0c
      HighlightCallsign,	//0d
      SwitchConfiguration,	//0e
      Configure,			//0f
      maximum_message_type_     // ONLY add new message types
                                // immediately before here
    };

  quint32 constexpr pulse {15}; // seconds

  //
  // NetworkMessage::Builder - build a message containing serialized Qt types
  //
  class Builder
    : public QDataStream
  {
  public:
    static quint32 constexpr magic {0xadbccbda}; // never change this
                                   

    // increment this if a newer Qt schema is required and add decode
    // logic to the Builder and Reader class implementations
#if QT_VERSION >= QT_VERSION_CHECK (5, 4, 0)
    static quint32 constexpr schema_number {3};
#elif QT_VERSION >= QT_VERSION_CHECK (5, 2, 0)
    static quint32 constexpr schema_number {2};//2
#elif QT_VERSION >= 0x040800
    static quint32 constexpr schema_number {1};//2.68  qt4=1 i v NetworkMessage.cpp
#else
    // Schema 1 (Qt_5_0) is broken
#error "Qt version nee to be addet LZ2HV"
#endif

    explicit Builder (QIODevice *, Type, QString const& id, quint32 schema);
    explicit Builder (QByteArray *, Type, QString const& id, quint32 schema);
    Builder (Builder const&) = delete;
    Builder& operator = (Builder const&) = delete;

  private:
    void common_initialization (Type type, QString const& id, quint32 schema);
  };

  //
  // NetworkMessage::Reader - read a message containing serialized Qt types
  //
  // Message  is as  per NetworkMessage::Builder  above, the  schema()
  // member  may be  used  to  determine the  schema  of the  original
  // message.
  //
  class Reader
    : public QDataStream
  {
  public:
    explicit Reader (QIODevice *);
    explicit Reader (QByteArray const&);
    Reader (Reader const&) = delete;
    Reader& operator = (Reader const&) = delete;
    ~Reader ();

    quint32 schema () const;
    Type type () const;
    QString id () const;

  private:
    class impl;
    pimpl<impl> m_;
  };
}

#endif
