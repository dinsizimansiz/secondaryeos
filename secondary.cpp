#include <eosiolib/eosio.hpp>
#include <eosiolib/dispatcher.hpp>
#include <eosiolib/multi_index.hpp>
#include <eosiolib/print.hpp>



namespace second
{
    using namespace eosio;
    

    class secondary : public contract
    {
    public:
        using contract::contract;
        secondary(account_name self):contract(self),_userdata(self,self) {}
        //@abi action
        void add(account_name s, uint8_t age,float balance)
        {
            _userdata.emplace(get_self(),[&](auto &yaprak){
                yaprak.id = _userdata.available_primary_key()   ;
                yaprak.age = age;
                yaprak.balance = balance;
            });
        }
    private:  

        //@abi table
        struct user 
        {
            account_name id;
            uint8_t age;
            float balance;

            auto primary_key() const {return id;}
            uint8_t getage() const {return age;}
            float getbalance() const {return balance;}
            EOSLIB_SERIALIZE(user,(id)(age)(balance));
        };
        typedef multi_index<N(user),user,
        indexed_by<N(byage),const_mem_fun<user,uint8_t,&user::getage>>,
        indexed_by<N(bybalance), const_mem_fun<user,float,&user::getbalance>>
        > userbase ; 
        userbase _userdata;
    };

    EOSIO_ABI(secondary,(add));
}