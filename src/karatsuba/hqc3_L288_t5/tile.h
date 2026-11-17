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
        for (uint32_t i = 0u; i < 36u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 0u]); }
        for (uint32_t i = 36u; i < 72u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 0u] ^ product[i - 36u]); }
        for (uint32_t i = 72u; i < 108u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 36u] ^ product[i - 72u]); }
        for (uint32_t i = 108u; i < 144u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 72u] ^ product[i - 108u]); }
        for (uint32_t i = 144u; i < 180u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 108u] ^ product[i - 144u]); }
        for (uint32_t i = 180u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 144u] ^ product[i - 180u]); }
        for (uint32_t i = 216u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 180u] ^ product[i - 216u]); }
        for (uint32_t i = 252u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 216u] ^ product[i - 252u]); }
        for (uint32_t i = 288u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 252u] ^ product[i - 288u]); }
        for (uint32_t i = 324u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 288u] ^ product[i - 324u]); }
        for (uint32_t i = 360u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 324u] ^ product[i - 360u]); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 360u] ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 396u] ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 432u] ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(0u); }
    }
};
template<> struct IntervalContribution<1u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 36u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 36u; i < 72u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 36u]); }
        for (uint32_t i = 72u; i < 108u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 36u] ^ product[i - 72u]); }
        for (uint32_t i = 108u; i < 144u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 72u] ^ product[i - 108u]); }
        for (uint32_t i = 144u; i < 180u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 108u] ^ product[i - 144u]); }
        for (uint32_t i = 180u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 144u] ^ product[i - 180u]); }
        for (uint32_t i = 216u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 180u] ^ product[i - 216u]); }
        for (uint32_t i = 252u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u] ^ product[i - 252u]); }
        for (uint32_t i = 288u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u] ^ product[i - 288u]); }
        for (uint32_t i = 324u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u] ^ product[i - 324u]); }
        for (uint32_t i = 360u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 324u] ^ product[i - 360u]); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u] ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<2u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 36u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 36u; i < 108u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 36u]); }
        for (uint32_t i = 108u; i < 180u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 108u]); }
        for (uint32_t i = 180u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 180u]); }
        for (uint32_t i = 252u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u]); }
        for (uint32_t i = 324u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 324u]); }
        for (uint32_t i = 396u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u]); }
        for (uint32_t i = 468u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u]); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u]); }
        for (uint32_t i = 684u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<3u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 72u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 72u; i < 108u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 72u]); }
        for (uint32_t i = 108u; i < 144u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 72u] ^ product[i - 108u]); }
        for (uint32_t i = 144u; i < 180u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 108u] ^ product[i - 144u]); }
        for (uint32_t i = 180u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 144u] ^ product[i - 180u]); }
        for (uint32_t i = 216u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 180u] ^ product[i - 216u]); }
        for (uint32_t i = 252u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u] ^ product[i - 252u]); }
        for (uint32_t i = 288u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u] ^ product[i - 288u]); }
        for (uint32_t i = 324u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u] ^ product[i - 324u]); }
        for (uint32_t i = 360u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 324u] ^ product[i - 360u]); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u] ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<4u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 108u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 108u; i < 144u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 108u]); }
        for (uint32_t i = 144u; i < 180u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 108u] ^ product[i - 144u]); }
        for (uint32_t i = 180u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 144u] ^ product[i - 180u]); }
        for (uint32_t i = 216u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 180u] ^ product[i - 216u]); }
        for (uint32_t i = 252u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u] ^ product[i - 252u]); }
        for (uint32_t i = 288u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u] ^ product[i - 288u]); }
        for (uint32_t i = 324u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u] ^ product[i - 324u]); }
        for (uint32_t i = 360u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 324u] ^ product[i - 360u]); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u] ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1296u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<5u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 108u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 108u; i < 180u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 108u]); }
        for (uint32_t i = 180u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 180u]); }
        for (uint32_t i = 252u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u]); }
        for (uint32_t i = 324u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 324u]); }
        for (uint32_t i = 396u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u]); }
        for (uint32_t i = 468u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u]); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u]); }
        for (uint32_t i = 684u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<6u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 72u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 72u; i < 108u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 72u]); }
        for (uint32_t i = 108u; i < 144u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 72u] ^ product[i - 108u]); }
        for (uint32_t i = 144u; i < 180u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 108u]); }
        for (uint32_t i = 180u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 216u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u]); }
        for (uint32_t i = 252u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u] ^ product[i - 252u]); }
        for (uint32_t i = 288u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u]); }
        for (uint32_t i = 324u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 360u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u]); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<7u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 108u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 108u; i < 144u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 108u]); }
        for (uint32_t i = 144u; i < 180u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 108u] ^ product[i - 144u]); }
        for (uint32_t i = 180u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 144u]); }
        for (uint32_t i = 216u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 252u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u]); }
        for (uint32_t i = 288u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u] ^ product[i - 288u]); }
        for (uint32_t i = 324u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u]); }
        for (uint32_t i = 360u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u] ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<8u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 108u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 108u; i < 180u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 108u]); }
        for (uint32_t i = 180u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 252u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u]); }
        for (uint32_t i = 324u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 396u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u]); }
        for (uint32_t i = 468u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 684u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<9u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 144u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 144u; i < 180u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 144u]); }
        for (uint32_t i = 180u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 144u] ^ product[i - 180u]); }
        for (uint32_t i = 216u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 180u] ^ product[i - 216u]); }
        for (uint32_t i = 252u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u] ^ product[i - 252u]); }
        for (uint32_t i = 288u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u] ^ product[i - 288u]); }
        for (uint32_t i = 324u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u] ^ product[i - 324u]); }
        for (uint32_t i = 360u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 324u] ^ product[i - 360u]); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u] ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<10u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 180u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 180u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 180u]); }
        for (uint32_t i = 216u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 180u] ^ product[i - 216u]); }
        for (uint32_t i = 252u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u] ^ product[i - 252u]); }
        for (uint32_t i = 288u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u] ^ product[i - 288u]); }
        for (uint32_t i = 324u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u] ^ product[i - 324u]); }
        for (uint32_t i = 360u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 324u] ^ product[i - 360u]); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u] ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u]); }
        for (uint32_t i = 1368u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<11u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 180u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 180u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 180u]); }
        for (uint32_t i = 252u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u]); }
        for (uint32_t i = 324u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 324u]); }
        for (uint32_t i = 396u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u]); }
        for (uint32_t i = 468u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u]); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u]); }
        for (uint32_t i = 684u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<12u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 216u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u]); }
        for (uint32_t i = 252u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u] ^ product[i - 252u]); }
        for (uint32_t i = 288u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u] ^ product[i - 288u]); }
        for (uint32_t i = 324u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u] ^ product[i - 324u]); }
        for (uint32_t i = 360u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 324u] ^ product[i - 360u]); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u] ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<13u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 252u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u]); }
        for (uint32_t i = 288u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u] ^ product[i - 288u]); }
        for (uint32_t i = 324u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u] ^ product[i - 324u]); }
        for (uint32_t i = 360u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 324u] ^ product[i - 360u]); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u] ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u]); }
        for (uint32_t i = 1440u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<14u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 252u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u]); }
        for (uint32_t i = 324u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 324u]); }
        for (uint32_t i = 396u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u]); }
        for (uint32_t i = 468u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u]); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u]); }
        for (uint32_t i = 684u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<15u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 216u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u]); }
        for (uint32_t i = 252u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u] ^ product[i - 252u]); }
        for (uint32_t i = 288u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u]); }
        for (uint32_t i = 324u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 360u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u]); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<16u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 252u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u]); }
        for (uint32_t i = 288u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u] ^ product[i - 288u]); }
        for (uint32_t i = 324u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u]); }
        for (uint32_t i = 360u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u] ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u]); }
        for (uint32_t i = 1368u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<17u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 252u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u]); }
        for (uint32_t i = 324u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 396u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u]); }
        for (uint32_t i = 468u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 684u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<18u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 144u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 144u; i < 180u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 144u]); }
        for (uint32_t i = 180u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 144u] ^ product[i - 180u]); }
        for (uint32_t i = 216u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 180u] ^ product[i - 216u]); }
        for (uint32_t i = 252u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u] ^ product[i - 252u]); }
        for (uint32_t i = 288u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u]); }
        for (uint32_t i = 324u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<19u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 180u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 180u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 180u]); }
        for (uint32_t i = 216u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 180u] ^ product[i - 216u]); }
        for (uint32_t i = 252u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u] ^ product[i - 252u]); }
        for (uint32_t i = 288u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u] ^ product[i - 288u]); }
        for (uint32_t i = 324u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u]); }
        for (uint32_t i = 360u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 936u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<20u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 180u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 180u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 180u]); }
        for (uint32_t i = 252u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u]); }
        for (uint32_t i = 324u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 468u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u]); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<21u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 216u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u]); }
        for (uint32_t i = 252u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u] ^ product[i - 252u]); }
        for (uint32_t i = 288u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u] ^ product[i - 288u]); }
        for (uint32_t i = 324u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u] ^ product[i - 324u]); }
        for (uint32_t i = 360u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 324u]); }
        for (uint32_t i = 396u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u]); }
        for (uint32_t i = 684u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<22u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 252u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u]); }
        for (uint32_t i = 288u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u] ^ product[i - 288u]); }
        for (uint32_t i = 324u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u] ^ product[i - 324u]); }
        for (uint32_t i = 360u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 324u] ^ product[i - 360u]); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u]); }
        for (uint32_t i = 432u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u]); }
        for (uint32_t i = 720u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 1008u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1296u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<23u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 252u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u]); }
        for (uint32_t i = 324u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 324u]); }
        for (uint32_t i = 396u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u]); }
        for (uint32_t i = 684u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<24u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 216u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 216u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u]); }
        for (uint32_t i = 252u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 216u] ^ product[i - 252u]); }
        for (uint32_t i = 288u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u]); }
        for (uint32_t i = 324u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<25u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 252u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u]); }
        for (uint32_t i = 288u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u] ^ product[i - 288u]); }
        for (uint32_t i = 324u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u]); }
        for (uint32_t i = 360u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 936u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<26u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 252u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 252u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 252u]); }
        for (uint32_t i = 324u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<27u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 288u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u]); }
        for (uint32_t i = 324u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u] ^ product[i - 324u]); }
        for (uint32_t i = 360u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 324u] ^ product[i - 360u]); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u] ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<28u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 324u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 324u]); }
        for (uint32_t i = 360u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 324u] ^ product[i - 360u]); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u] ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u]); }
        for (uint32_t i = 1512u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<29u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 324u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 324u]); }
        for (uint32_t i = 396u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u]); }
        for (uint32_t i = 468u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u]); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u]); }
        for (uint32_t i = 684u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<30u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 360u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u]); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u] ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<31u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u] ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u]); }
        for (uint32_t i = 1584u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<32u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 396u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u]); }
        for (uint32_t i = 468u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u]); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u]); }
        for (uint32_t i = 684u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<33u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 360u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u]); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<34u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u] ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u]); }
        for (uint32_t i = 1512u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<35u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 396u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u]); }
        for (uint32_t i = 468u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 684u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<36u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<37u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u]); }
        for (uint32_t i = 1656u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<38u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 468u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u]); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u]); }
        for (uint32_t i = 684u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<39u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u]); }
        for (uint32_t i = 1692u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<40u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u]); }
        for (uint32_t i = 1728u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<41u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u]); }
        for (uint32_t i = 684u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u]); }
        for (uint32_t i = 1692u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<42u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<43u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u]); }
        for (uint32_t i = 1656u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<44u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 684u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1548u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<45u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<46u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 936u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u]); }
        for (uint32_t i = 1512u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<47u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 468u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u]); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<48u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u]); }
        for (uint32_t i = 684u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<49u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u]); }
        for (uint32_t i = 720u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 1008u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1296u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u]); }
        for (uint32_t i = 1584u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<50u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u]); }
        for (uint32_t i = 684u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<51u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<52u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 936u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u]); }
        for (uint32_t i = 1512u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<53u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<54u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 288u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 288u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u]); }
        for (uint32_t i = 324u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 288u] ^ product[i - 324u]); }
        for (uint32_t i = 360u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 324u] ^ product[i - 360u]); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u] ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<55u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 324u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 324u]); }
        for (uint32_t i = 360u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 324u] ^ product[i - 360u]); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u] ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<56u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 324u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 324u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 324u]); }
        for (uint32_t i = 396u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u]); }
        for (uint32_t i = 468u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u]); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<57u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 360u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u]); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u] ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u]); }
        for (uint32_t i = 684u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<58u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u] ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u]); }
        for (uint32_t i = 720u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1296u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<59u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 396u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u]); }
        for (uint32_t i = 468u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u]); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u]); }
        for (uint32_t i = 684u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<60u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 360u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 360u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u]); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 360u] ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<61u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 396u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u]); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u] ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<62u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 396u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 396u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 396u]); }
        for (uint32_t i = 468u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<63u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<64u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u]); }
        for (uint32_t i = 792u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u]); }
        for (uint32_t i = 1368u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<65u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 468u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u]); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u]); }
        for (uint32_t i = 684u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<66u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 828u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<67u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 864u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u]); }
        for (uint32_t i = 1440u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<68u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u]); }
        for (uint32_t i = 684u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 828u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<69u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<70u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u]); }
        for (uint32_t i = 792u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u]); }
        for (uint32_t i = 1368u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<71u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 684u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<72u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 432u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 432u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u]); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 432u] ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<73u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 468u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u]); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u] ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<74u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 468u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 468u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 468u]); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<75u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u]); }
        for (uint32_t i = 684u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<76u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u]); }
        for (uint32_t i = 720u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1296u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<77u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u]); }
        for (uint32_t i = 684u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<78u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 504u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 504u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u]); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 504u] ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<79u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u] ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u]); }
        for (uint32_t i = 648u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<80u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 540u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 540u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 540u]); }
        for (uint32_t i = 612u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<81u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<82u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u]); }
        for (uint32_t i = 1800u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<83u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 612u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u]); }
        for (uint32_t i = 684u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u]); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<84u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u]); }
        for (uint32_t i = 1836u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<85u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u] ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u]); }
        for (uint32_t i = 1872u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<86u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 684u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u]); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u]); }
        for (uint32_t i = 1836u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<87u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<88u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u]); }
        for (uint32_t i = 1800u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<89u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 684u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1548u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<90u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u] ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u] ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u]); }
        for (uint32_t i = 1908u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<91u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u] ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u] ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u] ^ product[i - 1872u]); }
        for (uint32_t i = 1908u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1872u]); }
        for (uint32_t i = 1944u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<92u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u]); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u]); }
        for (uint32_t i = 1836u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u]); }
        for (uint32_t i = 1908u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<93u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u] ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u] ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u] ^ product[i - 1872u]); }
        for (uint32_t i = 1908u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1872u] ^ product[i - 1908u]); }
        for (uint32_t i = 1944u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1908u]); }
        for (uint32_t i = 1980u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<94u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u] ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u] ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u] ^ product[i - 1872u]); }
        for (uint32_t i = 1908u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1872u] ^ product[i - 1908u]); }
        for (uint32_t i = 1944u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1908u] ^ product[i - 1944u]); }
        for (uint32_t i = 1980u; i < 2016u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1944u]); }
        for (uint32_t i = 2016u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<95u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u]); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u]); }
        for (uint32_t i = 1836u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u]); }
        for (uint32_t i = 1908u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1908u]); }
        for (uint32_t i = 1980u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<96u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u] ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u]); }
        for (uint32_t i = 1908u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<97u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u] ^ product[i - 1872u]); }
        for (uint32_t i = 1908u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1872u]); }
        for (uint32_t i = 1944u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<98u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1548u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1836u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u]); }
        for (uint32_t i = 1908u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<99u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<100u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 936u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u]); }
        for (uint32_t i = 1512u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u]); }
        for (uint32_t i = 1800u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<101u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1620u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u]); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<102u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u]); }
        for (uint32_t i = 1836u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<103u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 1008u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1296u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u]); }
        for (uint32_t i = 1584u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u] ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u]); }
        for (uint32_t i = 1872u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<104u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u]); }
        for (uint32_t i = 1836u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<105u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<106u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 936u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u]); }
        for (uint32_t i = 1512u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u]); }
        for (uint32_t i = 1800u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<107u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<108u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u] ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u] ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u] ^ product[i - 1872u]); }
        for (uint32_t i = 1908u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1872u] ^ product[i - 1908u]); }
        for (uint32_t i = 1944u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1908u] ^ product[i - 1944u]); }
        for (uint32_t i = 1980u; i < 2016u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1944u] ^ product[i - 1980u]); }
        for (uint32_t i = 2016u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u]); }
        for (uint32_t i = 2052u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<109u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u] ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u] ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u] ^ product[i - 1872u]); }
        for (uint32_t i = 1908u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1872u] ^ product[i - 1908u]); }
        for (uint32_t i = 1944u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1908u] ^ product[i - 1944u]); }
        for (uint32_t i = 1980u; i < 2016u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1944u] ^ product[i - 1980u]); }
        for (uint32_t i = 2016u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u] ^ product[i - 2016u]); }
        for (uint32_t i = 2052u; i < 2088u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2016u]); }
        for (uint32_t i = 2088u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<110u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u]); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u]); }
        for (uint32_t i = 1836u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u]); }
        for (uint32_t i = 1908u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1908u]); }
        for (uint32_t i = 1980u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u]); }
        for (uint32_t i = 2052u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<111u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u] ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u] ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u] ^ product[i - 1872u]); }
        for (uint32_t i = 1908u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1872u] ^ product[i - 1908u]); }
        for (uint32_t i = 1944u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1908u] ^ product[i - 1944u]); }
        for (uint32_t i = 1980u; i < 2016u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1944u] ^ product[i - 1980u]); }
        for (uint32_t i = 2016u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u] ^ product[i - 2016u]); }
        for (uint32_t i = 2052u; i < 2088u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2016u] ^ product[i - 2052u]); }
        for (uint32_t i = 2088u; i < 2124u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2052u]); }
        for (uint32_t i = 2124u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<112u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u] ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u] ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u] ^ product[i - 1872u]); }
        for (uint32_t i = 1908u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1872u] ^ product[i - 1908u]); }
        for (uint32_t i = 1944u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1908u] ^ product[i - 1944u]); }
        for (uint32_t i = 1980u; i < 2016u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1944u] ^ product[i - 1980u]); }
        for (uint32_t i = 2016u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u] ^ product[i - 2016u]); }
        for (uint32_t i = 2052u; i < 2088u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2016u] ^ product[i - 2052u]); }
        for (uint32_t i = 2088u; i < 2124u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2052u] ^ product[i - 2088u]); }
        for (uint32_t i = 2124u; i < 2160u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2088u]); }
        for (uint32_t i = 2160u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<113u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u]); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u]); }
        for (uint32_t i = 1836u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u]); }
        for (uint32_t i = 1908u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1908u]); }
        for (uint32_t i = 1980u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u]); }
        for (uint32_t i = 2052u; i < 2124u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2052u]); }
        for (uint32_t i = 2124u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<114u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u] ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u]); }
        for (uint32_t i = 1908u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1944u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1944u]); }
        for (uint32_t i = 1980u; i < 2016u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1944u] ^ product[i - 1980u]); }
        for (uint32_t i = 2016u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u]); }
        for (uint32_t i = 2052u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<115u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u] ^ product[i - 1872u]); }
        for (uint32_t i = 1908u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1872u]); }
        for (uint32_t i = 1944u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1980u; i < 2016u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u]); }
        for (uint32_t i = 2016u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u] ^ product[i - 2016u]); }
        for (uint32_t i = 2052u; i < 2088u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2016u]); }
        for (uint32_t i = 2088u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<116u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1548u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1836u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u]); }
        for (uint32_t i = 1908u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1980u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u]); }
        for (uint32_t i = 2052u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<117u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u] ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u] ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u] ^ product[i - 1872u]); }
        for (uint32_t i = 1908u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1872u] ^ product[i - 1908u]); }
        for (uint32_t i = 1944u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1908u] ^ product[i - 1944u]); }
        for (uint32_t i = 1980u; i < 2016u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1944u] ^ product[i - 1980u]); }
        for (uint32_t i = 2016u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u] ^ product[i - 2016u]); }
        for (uint32_t i = 2052u; i < 2088u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2016u] ^ product[i - 2052u]); }
        for (uint32_t i = 2088u; i < 2124u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2052u] ^ product[i - 2088u]); }
        for (uint32_t i = 2124u; i < 2160u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2088u] ^ product[i - 2124u]); }
        for (uint32_t i = 2160u; i < 2196u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2124u]); }
        for (uint32_t i = 2196u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<118u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u] ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u] ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u] ^ product[i - 1872u]); }
        for (uint32_t i = 1908u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1872u] ^ product[i - 1908u]); }
        for (uint32_t i = 1944u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1908u] ^ product[i - 1944u]); }
        for (uint32_t i = 1980u; i < 2016u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1944u] ^ product[i - 1980u]); }
        for (uint32_t i = 2016u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u] ^ product[i - 2016u]); }
        for (uint32_t i = 2052u; i < 2088u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2016u] ^ product[i - 2052u]); }
        for (uint32_t i = 2088u; i < 2124u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2052u] ^ product[i - 2088u]); }
        for (uint32_t i = 2124u; i < 2160u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2088u] ^ product[i - 2124u]); }
        for (uint32_t i = 2160u; i < 2196u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2124u] ^ product[i - 2160u]); }
        for (uint32_t i = 2196u; i < 2232u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2160u]); }
        for (uint32_t i = 2232u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<119u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u]); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u]); }
        for (uint32_t i = 1836u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u]); }
        for (uint32_t i = 1908u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1908u]); }
        for (uint32_t i = 1980u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u]); }
        for (uint32_t i = 2052u; i < 2124u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2052u]); }
        for (uint32_t i = 2124u; i < 2196u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2124u]); }
        for (uint32_t i = 2196u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<120u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u] ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u] ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u] ^ product[i - 1872u]); }
        for (uint32_t i = 1908u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1872u] ^ product[i - 1908u]); }
        for (uint32_t i = 1944u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1908u] ^ product[i - 1944u]); }
        for (uint32_t i = 1980u; i < 2016u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1944u] ^ product[i - 1980u]); }
        for (uint32_t i = 2016u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u] ^ product[i - 2016u]); }
        for (uint32_t i = 2052u; i < 2088u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2016u] ^ product[i - 2052u]); }
        for (uint32_t i = 2088u; i < 2124u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2052u] ^ product[i - 2088u]); }
        for (uint32_t i = 2124u; i < 2160u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2088u] ^ product[i - 2124u]); }
        for (uint32_t i = 2160u; i < 2196u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2124u] ^ product[i - 2160u]); }
        for (uint32_t i = 2196u; i < 2232u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2160u] ^ product[i - 2196u]); }
        for (uint32_t i = 2232u; i < 2268u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2196u]); }
        for (uint32_t i = 2268u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<121u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u] ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u] ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u] ^ product[i - 1872u]); }
        for (uint32_t i = 1908u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1872u] ^ product[i - 1908u]); }
        for (uint32_t i = 1944u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1908u] ^ product[i - 1944u]); }
        for (uint32_t i = 1980u; i < 2016u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1944u] ^ product[i - 1980u]); }
        for (uint32_t i = 2016u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u] ^ product[i - 2016u]); }
        for (uint32_t i = 2052u; i < 2088u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2016u] ^ product[i - 2052u]); }
        for (uint32_t i = 2088u; i < 2124u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2052u] ^ product[i - 2088u]); }
        for (uint32_t i = 2124u; i < 2160u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2088u] ^ product[i - 2124u]); }
        for (uint32_t i = 2160u; i < 2196u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2124u] ^ product[i - 2160u]); }
        for (uint32_t i = 2196u; i < 2232u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2160u] ^ product[i - 2196u]); }
        for (uint32_t i = 2232u; i < 2268u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2196u] ^ product[i - 2232u]); }
        for (uint32_t i = 2268u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2232u]); }
    }
};
template<> struct IntervalContribution<122u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u]); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u]); }
        for (uint32_t i = 1836u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u]); }
        for (uint32_t i = 1908u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1908u]); }
        for (uint32_t i = 1980u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u]); }
        for (uint32_t i = 2052u; i < 2124u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2052u]); }
        for (uint32_t i = 2124u; i < 2196u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2124u]); }
        for (uint32_t i = 2196u; i < 2268u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2196u]); }
        for (uint32_t i = 2268u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<123u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u] ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u]); }
        for (uint32_t i = 1908u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1944u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1944u]); }
        for (uint32_t i = 1980u; i < 2016u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1944u] ^ product[i - 1980u]); }
        for (uint32_t i = 2016u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u]); }
        for (uint32_t i = 2052u; i < 2088u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 2088u; i < 2124u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2088u]); }
        for (uint32_t i = 2124u; i < 2160u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2088u] ^ product[i - 2124u]); }
        for (uint32_t i = 2160u; i < 2196u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2124u]); }
        for (uint32_t i = 2196u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<124u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u] ^ product[i - 1872u]); }
        for (uint32_t i = 1908u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1872u]); }
        for (uint32_t i = 1944u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1980u; i < 2016u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u]); }
        for (uint32_t i = 2016u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u] ^ product[i - 2016u]); }
        for (uint32_t i = 2052u; i < 2088u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2016u]); }
        for (uint32_t i = 2088u; i < 2124u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 2124u; i < 2160u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2124u]); }
        for (uint32_t i = 2160u; i < 2196u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2124u] ^ product[i - 2160u]); }
        for (uint32_t i = 2196u; i < 2232u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2160u]); }
        for (uint32_t i = 2232u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<125u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1548u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1836u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u]); }
        for (uint32_t i = 1908u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1980u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u]); }
        for (uint32_t i = 2052u; i < 2124u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 2124u; i < 2196u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2124u]); }
        for (uint32_t i = 2196u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<126u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1872u]); }
        for (uint32_t i = 1908u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1872u] ^ product[i - 1908u]); }
        for (uint32_t i = 1944u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1908u] ^ product[i - 1944u]); }
        for (uint32_t i = 1980u; i < 2016u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1944u] ^ product[i - 1980u]); }
        for (uint32_t i = 2016u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u]); }
        for (uint32_t i = 2052u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<127u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u]); }
        for (uint32_t i = 1512u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u]); }
        for (uint32_t i = 1800u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1908u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1908u]); }
        for (uint32_t i = 1944u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1908u] ^ product[i - 1944u]); }
        for (uint32_t i = 1980u; i < 2016u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1944u] ^ product[i - 1980u]); }
        for (uint32_t i = 2016u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u] ^ product[i - 2016u]); }
        for (uint32_t i = 2052u; i < 2088u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2016u]); }
        for (uint32_t i = 2088u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<128u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1620u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u]); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1908u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1908u]); }
        for (uint32_t i = 1980u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u]); }
        for (uint32_t i = 2052u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<129u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u]); }
        for (uint32_t i = 1836u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1944u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1944u]); }
        for (uint32_t i = 1980u; i < 2016u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1944u] ^ product[i - 1980u]); }
        for (uint32_t i = 2016u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u] ^ product[i - 2016u]); }
        for (uint32_t i = 2052u; i < 2088u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2016u] ^ product[i - 2052u]); }
        for (uint32_t i = 2088u; i < 2124u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2052u]); }
        for (uint32_t i = 2124u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<130u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1296u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u]); }
        for (uint32_t i = 1584u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u] ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u]); }
        for (uint32_t i = 1872u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1980u; i < 2016u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u]); }
        for (uint32_t i = 2016u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u] ^ product[i - 2016u]); }
        for (uint32_t i = 2052u; i < 2088u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2016u] ^ product[i - 2052u]); }
        for (uint32_t i = 2088u; i < 2124u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2052u] ^ product[i - 2088u]); }
        for (uint32_t i = 2124u; i < 2160u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2088u]); }
        for (uint32_t i = 2160u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<131u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u]); }
        for (uint32_t i = 1836u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1980u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u]); }
        for (uint32_t i = 2052u; i < 2124u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2052u]); }
        for (uint32_t i = 2124u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<132u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1944u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1944u]); }
        for (uint32_t i = 1980u; i < 2016u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1944u] ^ product[i - 1980u]); }
        for (uint32_t i = 2016u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u]); }
        for (uint32_t i = 2052u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<133u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u]); }
        for (uint32_t i = 1512u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u]); }
        for (uint32_t i = 1800u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1980u; i < 2016u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u]); }
        for (uint32_t i = 2016u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u] ^ product[i - 2016u]); }
        for (uint32_t i = 2052u; i < 2088u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 2016u]); }
        for (uint32_t i = 2088u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<134u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1980u; i < 2052u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1980u]); }
        for (uint32_t i = 2052u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<135u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<136u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u]); }
        for (uint32_t i = 1800u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<137u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u]); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<138u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u]); }
        for (uint32_t i = 1836u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<139u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1296u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u] ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u]); }
        for (uint32_t i = 1872u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<140u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1548u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u]); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u]); }
        for (uint32_t i = 1836u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<141u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<142u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u]); }
        for (uint32_t i = 1800u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<143u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1548u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<144u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u] ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u] ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u]); }
        for (uint32_t i = 1908u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<145u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u]); }
        for (uint32_t i = 1368u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u] ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u] ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u] ^ product[i - 1872u]); }
        for (uint32_t i = 1908u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1872u]); }
        for (uint32_t i = 1944u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<146u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1620u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u]); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u]); }
        for (uint32_t i = 1836u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u]); }
        for (uint32_t i = 1908u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<147u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u] ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u] ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u] ^ product[i - 1872u]); }
        for (uint32_t i = 1908u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1872u] ^ product[i - 1908u]); }
        for (uint32_t i = 1944u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1908u]); }
        for (uint32_t i = 1980u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<148u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u]); }
        for (uint32_t i = 1440u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u] ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u] ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u] ^ product[i - 1872u]); }
        for (uint32_t i = 1908u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1872u] ^ product[i - 1908u]); }
        for (uint32_t i = 1944u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1908u] ^ product[i - 1944u]); }
        for (uint32_t i = 1980u; i < 2016u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1944u]); }
        for (uint32_t i = 2016u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<149u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u]); }
        for (uint32_t i = 1836u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u]); }
        for (uint32_t i = 1908u; i < 1980u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1908u]); }
        for (uint32_t i = 1980u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<150u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u] ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u]); }
        for (uint32_t i = 1908u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<151u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u]); }
        for (uint32_t i = 1368u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u]); }
        for (uint32_t i = 1872u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u] ^ product[i - 1872u]); }
        for (uint32_t i = 1908u; i < 1944u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1872u]); }
        for (uint32_t i = 1944u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<152u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1836u; i < 1908u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1836u]); }
        for (uint32_t i = 1908u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<153u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<154u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u]); }
        for (uint32_t i = 1800u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<155u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1620u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u]); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<156u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u]); }
        for (uint32_t i = 1836u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<157u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1296u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u] ^ product[i - 1764u]); }
        for (uint32_t i = 1800u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u] ^ product[i - 1800u]); }
        for (uint32_t i = 1836u; i < 1872u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1800u]); }
        for (uint32_t i = 1872u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<158u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 1836u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1764u]); }
        for (uint32_t i = 1836u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<159u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u] ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<160u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1728u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u] ^ product[i - 1728u]); }
        for (uint32_t i = 1764u; i < 1800u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1728u]); }
        for (uint32_t i = 1800u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<161u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1692u; i < 1764u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1692u]); }
        for (uint32_t i = 1764u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<162u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 576u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 576u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u]); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 576u] ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<163u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 612u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u]); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u] ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<164u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 612u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 612u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 612u]); }
        for (uint32_t i = 684u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<165u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<166u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1296u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<167u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 684u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<168u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 648u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 648u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u]); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 648u] ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<169u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 684u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u] ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<170u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 684u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 684u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 684u]); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<171u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<172u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u]); }
        for (uint32_t i = 1368u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<173u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<174u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<175u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u]); }
        for (uint32_t i = 1440u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<176u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<177u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<178u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u]); }
        for (uint32_t i = 1368u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<179u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<180u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 720u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 720u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u]); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 720u] ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<181u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 756u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u] ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 936u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<182u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 756u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 756u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 756u]); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<183u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<184u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 1008u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1296u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<185u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<186u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 792u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 792u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u]); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 792u] ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<187u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u] ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 936u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<188u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 828u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 828u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 828u]); }
        for (uint32_t i = 900u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<189u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<190u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u]); }
        for (uint32_t i = 1512u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<191u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<192u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<193u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u]); }
        for (uint32_t i = 1584u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<194u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<195u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<196u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u]); }
        for (uint32_t i = 1512u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<197u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<198u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<199u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u]); }
        for (uint32_t i = 1656u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<200u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<201u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u]); }
        for (uint32_t i = 1692u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<202u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u] ^ product[i - 1620u]); }
        for (uint32_t i = 1656u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u] ^ product[i - 1656u]); }
        for (uint32_t i = 1692u; i < 1728u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1656u]); }
        for (uint32_t i = 1728u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<203u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 1692u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1620u]); }
        for (uint32_t i = 1692u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<204u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u] ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<205u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1584u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u] ^ product[i - 1584u]); }
        for (uint32_t i = 1620u; i < 1656u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1584u]); }
        for (uint32_t i = 1656u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<206u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1548u; i < 1620u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1548u]); }
        for (uint32_t i = 1620u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<207u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<208u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u]); }
        for (uint32_t i = 1512u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<209u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<210u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<211u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1296u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u] ^ product[i - 1476u]); }
        for (uint32_t i = 1512u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u] ^ product[i - 1512u]); }
        for (uint32_t i = 1548u; i < 1584u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1512u]); }
        for (uint32_t i = 1584u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<212u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 1548u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1476u]); }
        for (uint32_t i = 1548u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<213u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u] ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<214u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1440u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u] ^ product[i - 1440u]); }
        for (uint32_t i = 1476u; i < 1512u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1440u]); }
        for (uint32_t i = 1512u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<215u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1404u; i < 1476u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1404u]); }
        for (uint32_t i = 1476u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<216u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 864u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 864u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u]); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 864u] ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<217u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 900u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u] ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<218u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 900u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 900u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 900u]); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<219u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<220u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1296u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<221u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<222u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 936u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 936u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u]); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 936u] ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<223u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u] ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<224u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 972u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 972u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 972u]); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<225u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<226u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u]); }
        for (uint32_t i = 1368u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<227u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<228u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<229u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u] ^ product[i - 1332u]); }
        for (uint32_t i = 1368u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u] ^ product[i - 1368u]); }
        for (uint32_t i = 1404u; i < 1440u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1368u]); }
        for (uint32_t i = 1440u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<230u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 1404u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1332u]); }
        for (uint32_t i = 1404u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<231u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u] ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<232u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1296u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u] ^ product[i - 1296u]); }
        for (uint32_t i = 1332u; i < 1368u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1296u]); }
        for (uint32_t i = 1368u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<233u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1260u; i < 1332u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1260u]); }
        for (uint32_t i = 1332u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<234u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1008u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1008u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u]); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1008u] ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<235u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u] ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<236u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1044u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1044u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1044u]); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<237u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<238u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u] ^ product[i - 1188u]); }
        for (uint32_t i = 1224u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u] ^ product[i - 1224u]); }
        for (uint32_t i = 1260u; i < 1296u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1224u]); }
        for (uint32_t i = 1296u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<239u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 1260u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1188u]); }
        for (uint32_t i = 1260u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<240u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1080u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1080u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u]); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1080u] ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<241u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1152u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1152u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u] ^ product[i - 1152u]); }
        for (uint32_t i = 1188u; i < 1224u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1152u]); }
        for (uint32_t i = 1224u; i < 2304u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
    }
};
template<> struct IntervalContribution<242u> {
    template<class FIn, class FOut>
    static void run(FIn& in, FOut& out,
                    const uint32_t* __restrict product) {
        for (uint32_t i = 0u; i < 1116u; ++i)
        KARA_PIPELINE
        { out.write(in.read()); }
        for (uint32_t i = 1116u; i < 1188u; ++i)
        KARA_PIPELINE
        { out.write(in.read() ^ product[i - 1116u]); }
        for (uint32_t i = 1188u; i < 2304u; ++i)
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
