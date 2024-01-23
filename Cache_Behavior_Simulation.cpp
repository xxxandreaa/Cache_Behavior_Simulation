#include <iostream>
#include <fstream>
#include <cmath>
using namespace std;

class entry;

class entry
{
public:
    string tag;
    int nru;
    entry();
};

entry::entry()
{
    nru = 1;
    tag = "ccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccccc";
}

int main(int argc, char * argv[]) {

    ifstream  cache;
    cache.open(argv[1],ios::in);
    ifstream  refer;
    refer.open(argv[2],ios::in);
    ofstream fout;
    fout.open(argv[3],ios::out);

    string trash;
    int address_bit, block_size, cache_sets, associativity;
    cache >> trash >> address_bit;
    cache >> trash >> block_size;
    cache >> trash >> cache_sets;
    cache >> trash >> associativity;
 
    //output to fout
    fout << "Address bits: " << address_bit << endl;
    fout << "Block size: " << block_size << endl;
    fout << "Cache sets: " << cache_sets << endl;
    fout << "Associativity: " << associativity << endl;
    fout << endl;
    
    int offset_bit = 0, index_bit = 0;
    
    offset_bit = (int)log2(block_size);
    index_bit = (int)log2(cache_sets);
    
    if (offset_bit < 0)
    {
        offset_bit = 0;
    }
    if (index_bit < 0)
    {
        index_bit = 0;
    }
    
    fout << "Offset bit count: " << offset_bit << endl;
    fout << "Indexing bit count: " << index_bit << endl;
    fout << "Indexing bits:";
    
    //choose index bit
    string input[1000000];
    refer >> input[0];
    refer >> input[1];
    int size = 2;
    string help;
   
    while (true)
    {
        refer >> help;
        input[size] = help;
        size++;
        if (help[0] == '.')
            break;
    }
    size = size - 1;
    int chosen_bits = 0;
    chosen_bits = address_bit - offset_bit;
    
    
    
    
    
    //fout chosen bit
    for (int i = index_bit-1; i >= 0; i--)
    {
        fout << " " << offset_bit+i;
    }
    fout << endl << endl;
    
    // first line
    fout << input[0] << " " << input[1] << endl;
    
    // consruct class array cache set & accotativity, dynamic array
    entry** cache_array = new entry* [cache_sets];
    for (int i = 0; i < cache_sets; i++)
    {
        cache_array[i] = new entry[associativity];
    }
    
    //address use char
    string address;
    int tag_bit = 0;
    int miss = 0;
       
    while (true)
    {
        refer >> address;
        if (address[0] == '.')
        {
            fout << address << endl << endl;
            break;
        }
        
    // get tag
        tag_bit = address_bit - index_bit - offset_bit;
        
    // turn index to number
        int indexNum = 0;
        int base2 = 1;
        for (int i = index_bit-1; i >= 0; i--)
        {
            if (address[tag_bit+i] == '1')
            {
                indexNum += base2;
            }
            base2 = base2 * 2;
        }

    //compare with array, insert or replace in array
        
        int replaced = -1;
        bool hit = 0;
        for (int i = associativity-1; i >= 0; i--)
        {
            hit = 0;
            for (int j = 0; j < tag_bit; j++)
            {
                if (cache_array[indexNum][i].tag[j] == address[j])
                {
                    hit = 1;
                }
                else
                {
                    hit = 0;
                    if(cache_array[indexNum][i].nru == 1)
                    {
                        replaced = i;
                    }
                    break;
                }
            }

            if (hit == 1)
            {
                fout << address << " hit" << endl;
                cache_array[indexNum][i].nru = 0;
                break;
            }
        }
    
        if (hit == 0)
        {
            if (replaced == -1)
            {
                for (int i = 0; i < associativity; i++)
                {
                    cache_array[indexNum][i].nru = 1;
                }
                replaced = 0;
            }
        
            for (int i = 0; i < tag_bit; i++)
            {
                cache_array[indexNum][replaced].tag[i] = address[i];
            }
            
            cache_array[indexNum][replaced].nru = 0;
            fout << address << " miss" << endl;
            miss++;
        }
    }
    
    fout << "Total cache miss count: " << miss <<endl;
   
    for (int i = 0; i < cache_sets; i++)
    {
        delete [] cache_array[i];
    }
    delete [] cache_array;
    return 0;
}
