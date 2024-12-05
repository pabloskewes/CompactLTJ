#include <iostream>
#include <cltj_index_metatrie.hpp>
//#include <cltj_index_spo.hpp>

using namespace std;

using namespace std::chrono;
using timer = std::chrono::high_resolution_clock;

int main(int argc, char **argv){
    try{

        if(argc != 2){
            cout<< argv[0] << " <dataset>" <<endl;
            return 0;
        }

        std::string dataset = argv[1];
        std::string index_name = dataset + ".cltj";
        vector<cltj::spo_triple> D;

        std::ifstream ifs(dataset);
        uint32_t s, p , o;
        cltj::spo_triple spo;
        uint64_t i = 0;
        ifs >> s >> p >> o;
        do {
            //ifs >> s >> p >> o;
            spo[0] = s; spo[1] = p; spo[2] = o;
            D.emplace_back(spo);
            i++;
            if (i%10000000==0) std::cout << i << std::endl;
            ifs >> s >> p >> o;
        } while (!ifs.eof());

        D.shrink_to_fit();
        std::cout << "Dataset: " << 3*D.size()*sizeof(::uint32_t) << " bytes." << std::endl;
        fflush(stdout);
        //sdsl::memory_monitor::start();

        auto start = timer::now();
        cltj::cltj_index_metatrie<cltj::compact_trie_v3> index(D);
        auto stop = timer::now();

        //sdsl::memory_monitor::stop();

        sdsl::store_to_file(index, index_name);

        cout << "Index saved" << endl;
        cout << duration_cast<seconds>(stop-start).count() << " seconds." << endl;
        cout << /*sdsl::memory_monitor::peak()*/ sdsl::size_in_bytes(index) << " bytes." << endl;
        // ti.indexNewTable(file_name);

        // ind.save();
    }
    catch(const char *msg){
        cerr<<msg<<endl;
    }
    return 0;
}
