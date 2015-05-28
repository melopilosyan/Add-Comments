

#ifndef MSKSIGNALDISPETCHER_H_20_5_2012
#define MSKSIGNALDISPETCHER_H_20_5_2012


#include <QObject>
#include <MSKClasses.h>
#include <AuxiliaryFunctions.h>
class QWidget;


class MSKSignalDispatcher : public QObject
{

    Q_OBJECT

public:
    explicit MSKSignalDispatcher( QWidget* ui = 0 );
    ~MSKSignalDispatcher();

    void connectToMainUI( QWidget* ui );
    void connectToChatWidget( QWidget* widget );
    void disconnectWithChatWidget( QWidget* widget );

    void sendUpdateContactList()                          { emit updateContactListsignal();            }
    void sendLog( SEString log )                          { sendLog( QString( (const char*)log ) );    }
    void sendLog( const char* log )                       { sendLog( QString( log ) );                 }
    void sendLog( const QString& log )                    { emit logSignal( log );                     }
    void sendMsg( const QString& msg )                    { emit msgSignal( msg );                     }
    void sendChangeChatWindowTitle( QString title )                 { emit changeChatWindowTitle( title );                 }
    void sendNewConversationAppear( MSKConversation::Ref conv )    { emit newConversationAppear( conv );    }
    void sendContactAvailabilityChange( QString contact ) { emit contactAvailabilityChange( contact ); }

signals:
    void changeChatWindowTitle( QString );
    void updateContactListsignal();
    void logSignal( const QString& );
    void msgSignal( const QString& );
    void newConversationAppear( MSKConversation::Ref );
    void contactAvailabilityChange( QString );

};

#endif // MSKSIGNALDISPETCHER_H_20_5_2012
