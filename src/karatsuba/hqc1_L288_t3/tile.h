// Stream abstraction: input.read() -> uint32_t; output.write(uint32_t).
// Every array is caller-owned. All choices below are template specializations.
#ifndef KARA_TILE_H
#define KARA_TILE_H
#include "kara_ctrl.h"
#include "fold.h"
namespace kara {
template<uint32_t ENABLED> struct ForwardWord;
template<> struct ForwardWord<1u> {
    template<class Out> static void put(Out& out, uint32_t word) { out.write(word); }
};
template<> struct ForwardWord<0u> {
    template<class Out> static void put(Out&, uint32_t) {}
};
template<uint32_t SELECTED> struct SelectWord;
template<> struct SelectWord<1u> {
    static void put(uint32_t* __restrict local, uint32_t i, uint32_t word) {
        local[i] ^= word;
    }
};
template<> struct SelectWord<0u> {
    static void put(uint32_t*, uint32_t, uint32_t) {}
};
// Avoid a shift by 32 for a possible extra 64-bit frame padding segment.
template<uint32_t TILE, uint32_t SEG, uint32_t VALID> struct SegmentSelected;
template<uint32_t TILE, uint32_t SEG> struct SegmentSelected<TILE, SEG, 1u> {
    static const uint32_t VALUE = (TileMeta<TILE>::SEGMENT_MASK >> SEG) & 1u;
};
template<uint32_t TILE, uint32_t SEG> struct SegmentSelected<TILE, SEG, 0u> {
    static const uint32_t VALUE = 0u;
};
template<uint32_t TILE, uint32_t SEG, uint32_t END> struct ReceiveSegments {
    static const uint32_t START = SEG * SUB_WORDS;
    static const uint32_t COUNT = (FRAME_WORDS - START < SUB_WORDS)
                               ? FRAME_WORDS - START : SUB_WORDS;
    static const uint32_t SELECTED = SegmentSelected<TILE, SEG,
                                                     (SEG < (1u << SPLIT))>::VALUE;
    template<class In, class Out>
    static void run(In& in, Out& out, uint32_t* __restrict local) {
        for (uint32_t i = 0; i < COUNT; ++i)
        KARA_PIPELINE
        {
            const uint32_t word = in.read();
            ForwardWord<(TILE + 1u < G)>::put(out, word);
            SelectWord<SELECTED>::put(local, i, word);
        }
        ReceiveSegments<TILE, SEG + 1u, END>::run(in, out, local);
    }
};
template<uint32_t TILE, uint32_t END> struct ReceiveSegments<TILE, END, END> {
    template<class In, class Out> static void run(In&, Out&, uint32_t*) {}
};
template<uint32_t TILE, uint32_t INDEX, uint32_t END> struct AddOffsets {
    static void run(const uint32_t* __restrict product, uint32_t* __restrict C) {
        for (uint32_t i = 0; i < PRODUCT_WORDS; ++i)
        KARA_PIPELINE
        { C[TileOffset<TILE, INDEX>::WORD + i] ^= product[i]; }
        AddOffsets<TILE, INDEX + 1u, END>::run(product, C);
    }
};
template<uint32_t TILE, uint32_t END> struct AddOffsets<TILE, END, END> {
    static void run(const uint32_t*, uint32_t*) {}
};
template<uint32_t FIRST> struct AccumulateWord;
template<> struct AccumulateWord<1u> {
    template<class In> static uint32_t get(In&, uint32_t contribution) {
        return contribution;
    }
};
template<> struct AccumulateWord<0u> {
    template<class In> static uint32_t get(In& in, uint32_t contribution) {
        return in.read() ^ contribution;
    }
};
template<uint32_t TILE> struct IntervalContribution;
template<> struct IntervalContribution<0u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        (void)in; // Tile 0 has no F input stream.
        for (uint32_t i = 0u; i < 72u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 0u]); }
        for (uint32_t i = 72u; i < 144u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 0u] ^ product[i - 72u]); }
        for (uint32_t i = 144u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 72u] ^ product[i - 144u]); }
        for (uint32_t i = 216u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 144u] ^ product[i - 216u]); }
        for (uint32_t i = 288u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 216u] ^ product[i - 288u]); }
        for (uint32_t i = 360u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 288u] ^ product[i - 360u]); }
        for (uint32_t i = 432u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 360u] ^ product[i - 432u]); }
        for (uint32_t i = 504u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 432u] ^ product[i - 504u]); }
        for (uint32_t i = 576u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 504u]); }
        for (uint32_t i = 648u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(0u); }
    }
};
template<> struct IntervalContribution<1u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 72u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 72u; i < 144u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 72u]); }
        for (uint32_t i = 144u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 72u] ^ product[i - 144u]); }
        for (uint32_t i = 216u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 144u] ^ product[i - 216u]); }
        for (uint32_t i = 288u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u] ^ product[i - 288u]); }
        for (uint32_t i = 360u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u] ^ product[i - 360u]); }
        for (uint32_t i = 432u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 432u]); }
        for (uint32_t i = 504u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 504u]); }
        for (uint32_t i = 576u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u]); }
        for (uint32_t i = 720u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<2u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 72u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 72u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 72u]); }
        for (uint32_t i = 216u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u]); }
        for (uint32_t i = 360u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u]); }
        for (uint32_t i = 504u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 648u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<3u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 144u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 144u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 144u]); }
        for (uint32_t i = 216u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 144u] ^ product[i - 216u]); }
        for (uint32_t i = 288u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u] ^ product[i - 288u]); }
        for (uint32_t i = 360u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u] ^ product[i - 360u]); }
        for (uint32_t i = 432u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 432u]); }
        for (uint32_t i = 504u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 504u]); }
        for (uint32_t i = 576u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 648u]); }
        for (uint32_t i = 720u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u]); }
        for (uint32_t i = 792u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<4u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 216u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u]); }
        for (uint32_t i = 288u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u] ^ product[i - 288u]); }
        for (uint32_t i = 360u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u] ^ product[i - 360u]); }
        for (uint32_t i = 432u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 432u]); }
        for (uint32_t i = 504u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 504u]); }
        for (uint32_t i = 576u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 648u]); }
        for (uint32_t i = 720u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 720u]); }
        for (uint32_t i = 792u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u]); }
        for (uint32_t i = 864u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<5u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 216u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u]); }
        for (uint32_t i = 360u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u]); }
        for (uint32_t i = 504u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 648u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u]); }
        for (uint32_t i = 792u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<6u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 144u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 144u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 144u]); }
        for (uint32_t i = 216u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 144u] ^ product[i - 216u]); }
        for (uint32_t i = 288u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u]); }
        for (uint32_t i = 360u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 432u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u]); }
        for (uint32_t i = 504u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 504u]); }
        for (uint32_t i = 576u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 648u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<7u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 216u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u]); }
        for (uint32_t i = 288u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u] ^ product[i - 288u]); }
        for (uint32_t i = 360u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u]); }
        for (uint32_t i = 432u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 576u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u]); }
        for (uint32_t i = 720u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<8u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 216u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u]); }
        for (uint32_t i = 360u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 648u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<9u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 288u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u]); }
        for (uint32_t i = 360u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u] ^ product[i - 360u]); }
        for (uint32_t i = 432u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 432u]); }
        for (uint32_t i = 504u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 504u]); }
        for (uint32_t i = 576u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 648u]); }
        for (uint32_t i = 720u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 720u]); }
        for (uint32_t i = 792u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 792u]); }
        for (uint32_t i = 864u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 936u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<10u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 360u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u]); }
        for (uint32_t i = 432u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 432u]); }
        for (uint32_t i = 504u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 504u]); }
        for (uint32_t i = 576u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 648u]); }
        for (uint32_t i = 720u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 720u]); }
        for (uint32_t i = 792u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 792u]); }
        for (uint32_t i = 864u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 864u]); }
        for (uint32_t i = 936u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 1008u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<11u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 360u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u]); }
        for (uint32_t i = 504u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 648u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u]); }
        for (uint32_t i = 792u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 936u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<12u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 432u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u]); }
        for (uint32_t i = 504u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 504u]); }
        for (uint32_t i = 576u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 648u]); }
        for (uint32_t i = 720u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 720u]); }
        for (uint32_t i = 792u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 792u]); }
        for (uint32_t i = 864u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 864u]); }
        for (uint32_t i = 936u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 936u]); }
        for (uint32_t i = 1008u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 1080u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<13u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 576u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 648u]); }
        for (uint32_t i = 720u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 720u]); }
        for (uint32_t i = 792u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 792u]); }
        for (uint32_t i = 864u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 864u]); }
        for (uint32_t i = 936u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 936u]); }
        for (uint32_t i = 1008u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 1008u]); }
        for (uint32_t i = 1080u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
    }
};
template<> struct IntervalContribution<14u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 648u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u]); }
        for (uint32_t i = 792u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 936u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 1080u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<15u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 432u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u]); }
        for (uint32_t i = 504u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 504u]); }
        for (uint32_t i = 576u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 648u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 720u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u]); }
        for (uint32_t i = 792u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 792u]); }
        for (uint32_t i = 864u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 936u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<16u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 576u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u]); }
        for (uint32_t i = 720u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 792u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 864u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 864u]); }
        for (uint32_t i = 936u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 1008u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<17u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 648u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 792u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 936u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<18u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 288u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u]); }
        for (uint32_t i = 360u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u] ^ product[i - 360u]); }
        for (uint32_t i = 432u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 432u]); }
        for (uint32_t i = 504u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 504u]); }
        for (uint32_t i = 576u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 648u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<19u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 360u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u]); }
        for (uint32_t i = 432u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 432u]); }
        for (uint32_t i = 504u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 504u]); }
        for (uint32_t i = 576u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u]); }
        for (uint32_t i = 720u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<20u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 360u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u]); }
        for (uint32_t i = 504u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 648u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<21u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 432u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u]); }
        for (uint32_t i = 504u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 504u]); }
        for (uint32_t i = 576u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 648u]); }
        for (uint32_t i = 720u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u]); }
        for (uint32_t i = 792u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<22u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 576u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 648u]); }
        for (uint32_t i = 720u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 720u]); }
        for (uint32_t i = 792u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u]); }
        for (uint32_t i = 864u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<23u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 648u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u]); }
        for (uint32_t i = 792u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<24u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 432u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u]); }
        for (uint32_t i = 504u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 504u]); }
        for (uint32_t i = 576u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 648u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<25u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 576u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u]); }
        for (uint32_t i = 720u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<26u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 648u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<uint32_t TILE> struct Tile {
    template<class In, class Forward>
    static void receive(In& in, Forward& forward,
                        uint32_t* __restrict a, uint32_t* __restrict b) {
        for (uint32_t i = 0; i < SUB_WORDS; ++i)
        KARA_PIPELINE
        { a[i] = 0u; b[i] = 0u; }
        ReceiveSegments<TILE, 0u, INPUT_SEGMENTS>::run(in, forward, a);
        ReceiveSegments<TILE, 0u, INPUT_SEGMENTS>::run(in, forward, b);
    }
    static void compute(const uint32_t* __restrict a, const uint32_t* __restrict b,
                        uint32_t* __restrict product, uint32_t* __restrict scratch) {
        kara::compute(a, b, product, scratch);
    }
    static void contribute(const uint32_t* __restrict product,
                           uint32_t* __restrict C) {
        for (uint32_t i = 0; i < F_WORDS; ++i)
        KARA_PIPELINE
        { C[i] = 0u; }
        AddOffsets<TILE, 0u, TileMeta<TILE>::OFFSET_COUNT>::run(product, C);
    }
    template<class FIn, class FOut>
    static void accumulate(FIn& in, FOut& out, const uint32_t* __restrict C) {
        for (uint32_t i = 0; i < F_WORDS; ++i)
        KARA_PIPELINE
        { out.write(AccumulateWord<(TILE == 0u)>::get(in, C[i])); }
    }
    template<class FIn, class FOut>
    static void emit(FIn& in, FOut& out, const uint32_t* __restrict product,
                     uint32_t*) {
        IntervalContribution<TILE>::run(in, out, product);
    }
};
template<class FIn, class Out>
inline void fold_tile(FIn& in, Out& out, uint32_t* __restrict F_buffer,
                      uint32_t* __restrict out_buffer) {
    for (uint32_t i = 0; i < F_WORDS; ++i)
    KARA_PIPELINE
    { F_buffer[i] = in.read(); }
    fold(F_buffer, out_buffer);
    for (uint32_t i = 0; i < FRAME_WORDS; ++i)
    KARA_PIPELINE
    { out.write(out_buffer[i]); }
}
struct UnusedForward {};
template<class In, class Out>
inline void single_tile(In& in, Out& out,
                        uint32_t* __restrict a, uint32_t* __restrict b,
                        uint32_t* __restrict product, uint32_t* __restrict scratch,
                        uint32_t* __restrict out_buffer) {
    // SPLIT=0 only: product itself is F, so no second F or C array is needed.
    UnusedForward unused;
    Tile<0u>::receive(in, unused, a, b);
    Tile<0u>::compute(a, b, product, scratch);
    fold(product, out_buffer);
    for (uint32_t i = 0; i < FRAME_WORDS; ++i)
    KARA_PIPELINE
    { out.write(out_buffer[i]); }
}
} // namespace kara
#endif
