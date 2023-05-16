#ifndef SPHINXOBJECT_H
#define SPHINXOBJECT_H

#include <pocketsphinx.h>
#include "ps_alignment_internal.h"
#include "util/cmd_ln.h"

#include "lipsyncdoc.h"

class SphinxObject {
public:
    SphinxObject();
    ~SphinxObject();

    int align(ps_config_t *config, const char *file);
    int decode_single(ps_decoder_t *decoder, FILE *infile);
    void output_hyp(ps_endpointer_t *ep, ps_decoder_t *decoder, ps_alignment_t *alignment);
    int format_hyp(char *outptr, int len, ps_endpointer_t *ep, ps_decoder_t *decoder);
    int format_seg_align(char *outptr, int maxlen,
                         ps_alignment_iter_t *itor,
                         double utt_start, int frate,
                         logmath_t *lmath, int state_align);
    int format_align_iter(char *outptr, int maxlen,
                          ps_alignment_iter_t *itor, double utt_start, int frate, logmath_t *lmath);
    int format_seg(char *outptr, int len, ps_seg_t *seg,
                   double utt_start, int frate,
                   logmath_t *lmath);

    //void catch_sig(int signum);

    static inline int global_done = 0;

    void SetDocument(LipsyncDoc *doc);

    bin_mdef_t *mdef;
    dict_t *dict;
    dict2pid_t *d2p;
    ps_alignment_t *al;
    ps_alignment_iter_t *itor, *itor2;
    cmd_ln_t *config;
    int score, start, duration;

private:
    LipsyncDoc					*fDoc;

};


#endif // SPHINXOBJECT_H
