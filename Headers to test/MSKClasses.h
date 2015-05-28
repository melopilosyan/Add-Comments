/*
   Inherited classes:

        MSKTransfer      : public Transfer
        MSKParticipant   : public Participant
        MSKConversation  : public Conversation
        MSKContact       : public Contact
        MSKAccount       : public Account
        MSKContactGroup  : public ContactGroup
        MSKSkype         : public Skype
*/


#ifndef MSKCLASSES_H_20_5_2012
#define MSKCLASSES_H_20_5_2012


#include <QMetaType>
#include <AuxiliaryFunctions.h>
class MSKSignalDispatcher;



//////////////////////////////////////////////////////////////
//                     MSKTransfer
class MSKTransfer : public Transfer
{
public:
  typedef DRef<MSKTransfer, Transfer> Ref;
  typedef DRefs<MSKTransfer, Transfer> Refs;

  MSKTransfer(unsigned int oid, SERootObject* root);
  ~MSKTransfer();

  void accept();
  void OnChange(int prop);
};


//////////////////////////////////////////////////////////////
//                     MSKConversation
class MSKParticipant : public Participant
{

public:
    typedef DRef<MSKParticipant, Participant> Ref;
    typedef DRefs<MSKParticipant, Participant> Refs;

    MSKParticipant( int oid, SERootObject* root );
    ~MSKParticipant();

};

//////////////////////////////////////////////////////////////
//                     MSKConversation
class MSKConversation : public Conversation
{

public:
    typedef DRef<MSKConversation, Conversation> Ref;
    typedef DRefs<MSKConversation, Conversation> Refs;

    int id;
    // Just for Q_DECLARE_METATYPE
    MSKConversation():Conversation(0, 0) {}
    MSKConversation& operator=( const MSKConversation& ) { return *this; }
    // Real neededs
    MSKConversation( int oid, SERootObject* root );
    ~MSKConversation();

    void setGuiId( int id );
    void OnChange( int prop );
    void OnMessage( const MessageRef& message );

    QString identity();
    QString displayName();

private:
    static int idCounter;
    int guiId;

    MSKTransfer::Refs transferList;
};
Q_DECLARE_METATYPE(MSKConversation::Ref)


//////////////////////////////////////////////////////////////
//                     MSKContact
class MSKContact : public Contact
{

public:
    typedef DRef<MSKContact, Contact> Ref;
    typedef DRefs<MSKContact, Contact> Refs;

    MSKContact( int oid, SERootObject* root );
    ~MSKContact();

    QPixmap avatar();
    QString identity();
    QString moodText();
    QString displayName();
    Contact::AVAILABILITY availability();

    void OnChange( int prop );

private:
};

//////////////////////////////////////////////////////////////
//                     MSKAccount
class MSKAccount : public Account
{

public:
    typedef DRef<MSKAccount, Account> Ref;
    typedef DRefs<MSKAccount, Account> Refs;

    MSKAccount( int oid, SERootObject* root );
    ~MSKAccount();

    QString skypeName();
    void OnChange( int prop );

private:
    QString toqstring( uint pv );     // pv - property value
};


//////////////////////////////////////////////////////////////
//                     MSKContactGroup
class MSKContactGroup : public ContactGroup
{

public:
    typedef DRef<MSKContactGroup, ContactGroup> Ref;
    typedef DRefs<MSKContactGroup, ContactGroup> Refs;

    MSKContactGroup( int oid, SERootObject* root );
    ~MSKContactGroup();

    void OnChange( const ContactRef& contact );

private:

};

//////////////////////////////////////////////////////////////
//                     MSKSkype
class MSKSkype : public Skype
{

public:
    typedef DRef<MSKSkype, Skype> Ref;
    typedef DRefs<MSKSkype, Skype> Refs;

    MSKSkype( int num_thread = 1 );
    ~MSKSkype();

    MSKAccount::Ref account;
    Conversation::Refs inbox;
    MSKContact::Refs allBuddies;
    MSKContactGroup::Ref allBuddiesGroup;

    void getContactsList();
    void getConversationList();
    MSKConversation::Ref inboxConversatioById( int convId, bool& ok );
    void login(
            const SEString& login,
            const SEString& passwd,
            bool savePasswd,
            bool saveDataLocally
    );
    void OnConversationListChange(
            const ConversationRef& conversation,
            const Conversation::LIST_TYPE& type,
            const bool &added
    );

    MSKContact::Refs contacts() const        { return allBuddies;                       }
    Account*      newAccount( int oid )      { return new MSKAccount( oid, this );      }
    Contact*      newContact( int oid )      { return new MSKContact( oid, this );      }
    Conversation* newConversation( int oid ) { return new MSKConversation( oid, this ); }
    ContactGroup* newContactGroup( int oid ) { return new MSKContactGroup( oid, this ); }
};

extern MSKSkype* skype;
extern MSKSignalDispatcher* dispatcher;


#endif // MSKCKASSES_H_20_5_2012
