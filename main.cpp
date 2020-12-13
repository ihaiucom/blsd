#include <iostream>
#include <cstdint>
#include <fstream>
#include <zlib.h>
#include <vector>
#include <cassert>

struct BLSHeader
{
    uint32_t magic; // FourCC-style magic, "GXVS", "GXPS", etc. Character order reversed in-file. "GXSH" in all new WoD
                    // shaders.
    uint32_t version; // version, 0x10003 in WotLK, 0x10004 in Cata/MoP/WoD, 0x10005 in BfA (25902), 0x10006 in BfA
                      // (25976), 01000b (SL?)
    uint32_t permutationCount; // (from old definition)
    uint32_t nShaders; // the expected number of shaders in this file (this is the number
                       // of distinct shaders that will be found if you inspect the file after decompression)
    uint32_t ofsCompressedChunks; // offset to array of offsets to compressed chunks
    uint32_t nCompressedChunks; // number of compressed chunks
    uint32_t ofsCompressedData; // offset to the start of the compressed (zlib) data
                                // bytes (all offsets from the array above are offset by this much)

    bool IsMagicValid() const;
};

void help()
{
    std::cerr << "Usage: blsd shader.bls\n";
}

void dump( const std::vector<uint8_t> &buff )
{
    bool mainFunc = false;
    int bracketsOpened = 0;
    bool shaderCode = false;

    for( auto i = 0u; i < buff.size(); i++ )
    {
        if( buff[i] == '#' )
        {
            if( std::string( buff.begin() + i, buff.begin() + i + 8 ) == "#version" )
            {
                shaderCode = true;
            }
        }

        if( shaderCode )
        {
            std::cout << buff[i];
            if( buff[i] == 'v' && !mainFunc )
            {
                if( std::string( buff.begin() + i, buff.begin() + i + 11 ) == "void main()" )
                {
                    mainFunc = true;
                }
            }

            if( buff[i] == '{' && mainFunc )
            {
                bracketsOpened++;
            }
            if( buff[i] == '}' && mainFunc )
            {
                bracketsOpened--;

                if( !bracketsOpened && mainFunc )
                {
                    mainFunc = false;
                    shaderCode = false;

                    std::cout << '\n';
                }
            }
        }
    }
}

int main( int argc, char *argv[] )
{
    if( argc != 2 )
    {
        help();

        return 0;
    }

    z_stream s{};

    s.zalloc = nullptr;
    s.zfree = nullptr;
    s.opaque = nullptr;
    auto r = inflateInit( &s );
    if( r != Z_OK )
    {
        std::cerr << "Error init zlib\n";
        return 1;
    }

    std::string fileName( argv[1] );

    std::ifstream file( fileName, std::fstream::in | std::fstream::binary );
    if( !file )
    {
        std::cerr << "Error open '" << fileName << "' file\n";

        return 1;
    }


    BLSHeader header;
    file.read( reinterpret_cast<char *>( &header ), sizeof( header ) );
    if( !header.IsMagicValid() )
    {
        std::cerr << "BLSHeader: Error magic value\n";

        return 1;
    }

    auto fsize = file.tellg();
    file.seekg( 0, std::ios::end );
    fsize = file.tellg() - fsize;
    file.seekg( header.ofsCompressedData );

    std::vector<uint8_t> compressedData;
    compressedData.resize( uint32_t( fsize ) );

    file.read( reinterpret_cast<char *>( compressedData.data() ), fsize );

    s.next_in = compressedData.data();
    s.avail_in = compressedData.size();

    const auto buffSize = 1024 * 1024;

    std::vector<uint8_t> scratch;
    scratch.resize( 1024 * 1024 );
    std::vector<uint8_t> out_data;
    out_data.reserve( 4 * 1024 * 1024 );

    int flushValue = 0;

    while( s.avail_in > 0 )
    {
        do
        {
            s.avail_out = buffSize;
            s.next_out = scratch.data();
            flushValue = inflate( &s, Z_NO_FLUSH );

            if( flushValue == Z_NEED_DICT || flushValue == Z_DATA_ERROR || flushValue == Z_MEM_ERROR )
            {
                inflateEnd( &s );
                break;
            }
            assert( flushValue == Z_OK || flushValue == Z_STREAM_END );

            uint32_t nbytes = buffSize - s.avail_out;
            out_data.insert( out_data.end(), scratch.begin(), scratch.begin() + nbytes );
        } while( !s.avail_out );

        if( inflateReset( &s ) == Z_STREAM_ERROR )
        {
            break;
        }
    }

    dump( out_data );

    inflateEnd( &s );
}

bool BLSHeader::IsMagicValid() const
{
    const uint32_t GXSH = int( 'G' ) << 24 | int( 'X' ) << 16 | int( 'S' ) << 8 | int( 'H' );
    return magic == GXSH;
}
