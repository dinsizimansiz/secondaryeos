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
        secondary(account_name self):contract(self),userdata(self,self) {}
        //@abi action
        void add(account_name s, uint64_t age,uint64_t balance)
        {
            require_auth(s);
            auto it = userdata.find(s);
            eosio_assert(it == userdata.end(),"User  found.");
            userdata.emplace(get_self(),[&](auto &yaprak){
                yaprak.id = userdata.available_primary_key()   ;
                yaprak.age = age;
                yaprak.balance = balance;
            });
        }
        //@abi action
        void del(account_name self)
        {
            require_auth(self);
            auto it = userdata.find(self);
            eosio_assert(it != userdata.end(),"User cannot be found");
            userdata.erase(it);
        }
        //@abi action
        void sortage(account_name self,uint64_t minage,uint64_t maxage)
        {
            require_auth(self);
            auto agex = userdata.get_index<N(byage)>();
            for(const auto& item : agex)
            {
                auto age = item.getage();
                if(age >= minage && age <= maxage)
                    print(age );
            }
        }
        //@abi action
        void sortbalance(account_name self,uint64_t minbalance,uint64_t maxbalance)
        {
            require_auth(self);
            auto balancex = userdata.get_index<N(bybalance)>();
            for(const auto& item : balancex)
            {
                auto balance = item.getbalance();
                if(balance >= minbalance && balance <= maxbalance)
                    print(balance );
            }
        }
        //@abi action
        void getplayer(const account_name search)
        {
            
            auto it = this->userdata.find(search);
            eosio_assert(it != userdata.end(),"Cannot get the player.");
            struct user usher = this->userdata.get(search);
            print(usher.primary_key()," has ",usher.getbalance(),"at his age ",usher.getage());
            

        }
    private:  

        //@abi table
        struct user 
        {
            account_name id;
            uint64_t age;
            uint64_t balance;

            account_name primary_key() const {return id;}
            uint64_t getage() const {return age;}
            uint64_t getbalance() const {return balance;}
            EOSLIB_SERIALIZE(user,(id)(age)(balance));
        };
        typedef multi_index<N(user),user,
        indexed_by<N(byage),const_mem_fun<user,uint64_t,&user::getage>>,
        indexed_by<N(bybalance), const_mem_fun<user,uint64_t,&user::getbalance>>
        > userbase ; 
        userbase userdata;
    };

    EOSIO_ABI(secondary,(add)(del)(sortage)(sortbalance)(getplayer));
}