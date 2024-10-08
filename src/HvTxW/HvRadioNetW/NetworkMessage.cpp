/* The algorithms, source code, look-and-feel of WSJT-X and related
 * programs, and protocol specifications for the modes FSK441, FT8, JT4,
 * JT6M, JT9, JT65, JTMS, QRA64, ISCAT, MSK144, are Copyright © 2001-2017
 * by one or more of the following authors: Joseph Taylor, K1JT; Bill
 * Somerville, G4WJS; Steven Franke, K9AN; Nico Palermo, IV3NWV; Greg Beam,
 * KI7MT; Michael Black, W9MDB; Edson Pereira, PY2SDR; Philip Karn, KA9Q;
 * and other members of the WSJT Development Group.
 *
 * MSHV NetworkMessage 
 * Rewritten into C++ and modified by Hrisimir Hristov, LZ2HV 2015-2019
 * May be used under the terms of the GNU General Public License (GPL)
 */  

#include "NetworkMessage.h"

//#include <exception>

#include <QString>
#include <QByteArray>

#include "pimpl_impl.h"

//#include <QtGui>
namespace NetworkMessage
{
  Builder::Builder (QIODevice * device, Type type, QString const& id, quint32 schema)
    : QDataStream {device}
  {
    common_initialization (type, id, schema);
  }

  Builder::Builder (QByteArray * a, Type type, QString const& id, quint32 schema)
    : QDataStream {a, QIODevice::WriteOnly}
  {
    common_initialization (type, id, schema);
  }

  void Builder::common_initialization (Type type, QString const& id, quint32 schema)
  {
    if (schema <= 1)//2.68 qt4=1
      {
        //setVersion (QDataStream::Qt_5_0); // Qt schema version
        setVersion (QDataStream::Qt_4_8); // Qt4 schema version
      }
#if QT_VERSION >= QT_VERSION_CHECK (5, 2, 0)
    else if (schema <= 2)//2
      {
        setVersion (QDataStream::Qt_5_2); // Qt schema version
      }
#endif
#if QT_VERSION >= QT_VERSION_CHECK (5, 4, 0)
    else if (schema <= 3)
      {
        setVersion (QDataStream::Qt_5_4); // Qt schema version
      }
#endif
    else
      {
        //throw std::runtime_error {"Unrecognized message schema"};
      }

    // the following two items assume that the quint32 encoding is
    // unchanged over QDataStream versions
    *this << magic;
    *this << schema;
    *this << static_cast<quint32> (type) << id.toUtf8 ();
  }

  class Reader::impl
  {
  public:
    void common_initialization (Reader * parent)
    {
      quint32 magic;
      *parent >> magic;
      if (magic != Builder::magic)
        {
          //throw std::runtime_error {"Invalid message format"};
        }
      *parent >> schema_;
      if (schema_ > Builder::schema_number)
        {
          //throw std::runtime_error {"Unrecognized message schema"};
        }
      if (schema_ <= 1)//2.68 qt4=1
        {
          //parent->setVersion (QDataStream::Qt_5_0);//Qt_4_8
          parent->setVersion (QDataStream::Qt_4_8);
        }
#if QT_VERSION >= QT_VERSION_CHECK (5, 2, 0)
      else if (schema_ <= 2)
        {
          parent->setVersion (QDataStream::Qt_5_2);
        }
#endif
#if QT_VERSION >= QT_VERSION_CHECK (5, 4, 0)
      else if (schema_ <= 3)
        {
          parent->setVersion (QDataStream::Qt_5_4);
        }
#endif
      quint32 type;
      *parent >> type >> id_;
      if (type >= maximum_message_type_)
        {
          //throw std::runtime_error {"Unrecognized message type"};
        }
      type_ = static_cast<Type> (type);
    }

    quint32 schema_;
    Type type_;
    QByteArray id_;
  };

  Reader::Reader (QIODevice * device)
    : QDataStream {device}
  {
    m_->common_initialization (this);
    //qDebug()<<"Reader ";
  }

  Reader::Reader (QByteArray const& a)
    : QDataStream {a}
  {
    m_->common_initialization (this);
  }

  Reader::~Reader ()
  {
  	//qDebug()<<"Reader Destroy";
  }

  quint32 Reader::schema () const
  {
    return m_->schema_;
  }

  Type Reader::type () const
  {
    return static_cast<Type> (m_->type_);
  }

  QString Reader::id () const
  {
    return QString::fromUtf8 (m_->id_);
  }
}
