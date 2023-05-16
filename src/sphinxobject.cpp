#include "sphinxobject.h"
#include "util/ckd_alloc.h"
#include "config_macro.h"

#include <signal.h>





SphinxObject::SphinxObject()
{
    bin_mdef_t *mdef;
    dict_t *dict;
    dict2pid_t *d2p;
    ps_alignment_t *al;
    cmd_ln_t *config;

    config = ps_config_parse_json(NULL,
                                      "dict: \"" "G:/CrimEngineDX11/CrimEngine-github/Papagayo/src/build-Papagayo_sphinx-Desktop_Qt_5_15_2_MSVC2019_64bit-Debug/debug/model/en-us/cmudict-en-us.dict\", "
                                      "fdict: \"" "G:/CrimEngineDX11/CrimEngine-github/Papagayo/src/build-Papagayo_sphinx-Desktop_Qt_5_15_2_MSVC2019_64bit-Debug/debug/model/en-us/en-us/noisedict\"");

    ps_config_set_bool(config, "phone_align", TRUE);
    mdef = bin_mdef_read(NULL, "G:/CrimEngineDX11/CrimEngine-github/Papagayo/src/build-Papagayo_sphinx-Desktop_Qt_5_15_2_MSVC2019_64bit-Debug/debug/model/en-us/en-us/mdef");
    dict = dict_init(config, mdef);
    d2p = dict2pid_build(mdef, dict);

    al = ps_alignment_init(d2p);



}

SphinxObject::~SphinxObject()
{
    if(al)
    {
        ps_alignment_free(al);
    }
    if(dict)
    {
        dict_free(dict);
    }
    if(d2p)
    {
        dict2pid_free(d2p);
    }
    if(mdef)
    {
        bin_mdef_free(mdef);
    }
    if(config)
    {
        ps_config_free(config);
    }

};

void SphinxObject::SetDocument(LipsyncDoc *doc)
{
    fDoc = doc;
}

int SphinxObject::align(ps_config_t *config, const char *file)
{
    int rv = 0, is_stdin = FALSE;
    ps_decoder_t *decoder = NULL;
    //const char *text = NULL;
    FILE *fh = NULL;
    std::string test = "it has come to my attention that some have lately called me a collaborator as if such a term were shameful";

    /* Please do not use bestpath for alignment. */
    ps_config_set_bool(config, "bestpath", FALSE);
    ps_config_set_str(config, "lm", NULL);
   /* if (0 == strcmp(inputs[0], "-")) {
        is_stdin = TRUE;
        fh = stdin;
    }*/
    if ((fh = fopen(file, "rb")) == NULL) {
        E_ERROR_SYSTEM("Failed to open %s for input", file);
        goto error_out;
    }
    if ((rv = ps_config_soundfile(config, fh, file)) < 0)
        goto error_out;
    if ((decoder = ps_init(config)) == NULL) {
        E_FATAL("PocketSphinx decoder init failed\n");
        rv = -1;
        goto error_out;
    }
    //text =


    if ((rv = ps_set_align_text(decoder,  test.c_str())) < 0)
        goto error_out;


    /*if ((rv = ps_set_align_text(decoder,  fDoc->fCurrentVoice->fText.toStdString().c_str())) < 0)
        goto error_out;*/

    rv = decode_single(decoder, fh);
    /* Fall through intentionally. */
error_out:
    if (fh && !is_stdin)
        fclose(fh);
    /*if (text)
        ckd_free(text);*/
    if (decoder)
        ps_free(decoder);
    return rv;
}


int SphinxObject::decode_single(ps_decoder_t *decoder, FILE *infile)
{
    ps_alignment_t *alignment = NULL;
    size_t data_size, block_size;
    short *data, *ptr;
    int rv = 0;

    data_size = 65536;
    block_size = 2048;
    ptr = data = (short *)ckd_calloc(data_size, sizeof(*data));

    while (!global_done) {
        size_t len;
        if ((size_t)(ptr + block_size - data) > data_size) {
            len = ptr - data;
            data_size *= 2;
            data = (short * )ckd_realloc(data, data_size * sizeof(*data));
            ptr = data + len;
        }
        len = fread(ptr, sizeof(*ptr), block_size, infile);
        if (len == 0) {
            if (feof(infile))
                break;
            else {
                E_ERROR_SYSTEM("Failed to read %d bytes\n",
                               sizeof(*ptr) * block_size);
                rv = -1;
                goto error_out;
            }
        }
        ptr += len;
    }
    if ((rv = ps_start_utt(decoder)) < 0)
        goto error_out;
    if ((rv = ps_process_raw(decoder, data, ptr - data, FALSE, TRUE)) < 0) {
        E_ERROR("ps_process_raw() failed\n");
        goto error_out;
    }
    if ((rv = ps_end_utt(decoder)) < 0)
        goto error_out;
    if (ps_set_alignment(decoder, NULL) < 0)
        goto error_out;
    if ((rv = ps_start_utt(decoder)) < 0)
        goto error_out;
    if ((rv = ps_process_raw(decoder, data, ptr - data, FALSE, TRUE)) < 0) {
        E_ERROR("ps_process_raw() failed\n");
        goto error_out;
    }
    if ((rv = ps_end_utt(decoder)) < 0)
        goto error_out;
    if ((alignment = ps_get_alignment(decoder)) == NULL)
        goto error_out;
    ps_activate_search(decoder, NULL);
    output_hyp(NULL, decoder, alignment);
    /* Fall through intentionally */
error_out:
    ckd_free(data);
    return rv;
}


void SphinxObject::output_hyp(ps_endpointer_t *ep, ps_decoder_t *decoder, ps_alignment_t *alignment)
{
    logmath_t *lmath;
    char *hyp_json, *ptr;
    int frate;
    int maxlen, len;
    double st;
    int state_align = true;

    maxlen = format_hyp(NULL, 0, ep, decoder);
    maxlen += 6; /* "w":,[ */
    lmath = ps_get_logmath(decoder);
    frate = ps_config_int(ps_get_config(decoder), "frate");
    if (ep == NULL)
        st = 0.0;
    else
        st = ps_endpointer_speech_start(ep);
    if (alignment) {
        ps_alignment_iter_t *itor = ps_alignment_words(alignment);
        if (itor == NULL)
            maxlen++; /* ] at end */
        for (; itor; itor = ps_alignment_iter_next(itor)) {
            maxlen += format_seg_align(NULL, 0, itor, st, frate,
                                       lmath, state_align);
            maxlen++; /* , or ] at end */
        }
    }
    else {
        ps_seg_t *itor = ps_seg_iter(decoder);
        if (itor == NULL)
            maxlen++; /* ] at end */
        for (; itor; itor = ps_seg_next(itor)) {
            maxlen += format_seg(NULL, 0, itor, st, frate, lmath);
            maxlen++; /* , or ] at end */
        }
    }
    maxlen++; /* final } */
    maxlen++; /* trailing \0 */

    ptr = hyp_json = (char *)ckd_calloc(maxlen, 1);
    len = maxlen;
    len = format_hyp(hyp_json, len, ep, decoder);
    ptr += len;
    maxlen -= len;

    assert(maxlen > 6);
    memcpy(ptr, ",\"w\":[", 6);
    ptr += 6;
    maxlen -= 6;

    if (alignment) {
        ps_alignment_iter_t *itor;
        for (itor = ps_alignment_words(alignment); itor;
             itor = ps_alignment_iter_next(itor)) {
            assert(maxlen > 0);
            len = format_seg_align(ptr, maxlen, itor, st, frate, lmath,
                                   state_align);
            ptr += len;
            maxlen -= len;
            *ptr++ = ',';
            maxlen--;
        }
    }
    else {
        ps_seg_t *itor = ps_seg_iter(decoder);
        if (itor == NULL) {
            *ptr++ = ']'; /* Gets overwritten below... */
            maxlen--;
        }
        for (; itor; itor = ps_seg_next(itor)) {
            assert(maxlen > 0);
            len = format_seg(ptr, maxlen, itor, st, frate, lmath);
            ptr += len;
            maxlen -= len;
            *ptr++ = ',';
            maxlen--;
        }
    }
    --ptr;
    *ptr++ = ']';
    assert(maxlen == 2);
    *ptr++ = '}';
    --maxlen;
    *ptr = '\0';
    puts(hyp_json);
    ckd_free(hyp_json);
}

#define HYP_FORMAT "{\"b\":%.3f,\"d\":%.3f,\"p\":%.3f,\"t\":\"%s\""

int SphinxObject::format_hyp(char *outptr, int len, ps_endpointer_t *ep, ps_decoder_t *decoder)
{
    logmath_t *lmath;
    double prob, st, et;
    const char *hyp;

    lmath = ps_get_logmath(decoder);
    prob = logmath_exp(lmath, ps_get_prob(decoder));
    if (ep == NULL) {
        st = 0.0;
        et = (double)ps_get_n_frames(decoder)
            / ps_config_int(ps_get_config(decoder), "frate");
    }
    else {
        st = ps_endpointer_speech_start(ep);
        et = ps_endpointer_speech_end(ep);
    }
    hyp = ps_get_hyp(decoder, NULL);
    if (hyp == NULL)
        hyp = "";
    return snprintf(outptr, len, HYP_FORMAT, st, et - st, prob, hyp);
}

int SphinxObject::format_seg_align(char *outptr, int maxlen,
                 ps_alignment_iter_t *itor,
                 double utt_start, int frate,
                 logmath_t *lmath, int state_align)
{
    ps_alignment_iter_t *pitor;
    int len = 0, hyplen;

    hyplen = format_align_iter(outptr, maxlen,
                               itor, utt_start, frate, lmath);
    len += hyplen;
    if (outptr)
        outptr += hyplen;
    if (maxlen)
        maxlen -= hyplen;

    len += 6; /* "w":,[ */
    if (outptr) {
        memcpy(outptr, ",\"w\":[", 6);
        outptr += 6;
    }
    if (maxlen)
        maxlen -= 6;

    pitor = ps_alignment_iter_children(itor);
    while (pitor != NULL) {
        hyplen = format_align_iter(outptr, maxlen,
                                   pitor, utt_start, frate, lmath);
        len += hyplen;
        if (outptr)
            outptr += hyplen;
        if (maxlen)
            maxlen -= hyplen;

        /* FIXME: refactor with recursion, someday */
        if (state_align) {
            ps_alignment_iter_t *sitor = ps_alignment_iter_children(pitor);
            len += 6; /* "w":,[ */
            if (outptr) {
                memcpy(outptr, ",\"w\":[", 6);
                outptr += 6;
            }
            if (maxlen)
                maxlen -= 6;
            while (sitor != NULL) {
                hyplen = format_align_iter(outptr, maxlen,
                                           sitor, utt_start, frate, lmath);
                len += hyplen;
                if (outptr)
                    outptr += hyplen;
                if (maxlen)
                    maxlen -= hyplen;

                len++; /* } */
                if (outptr)
                    *outptr++ = '}';
                if (maxlen)
                    maxlen--;
                sitor = ps_alignment_iter_next(sitor);
                if (sitor != NULL) {
                    len++;
                    if (outptr)
                        *outptr++ = ',';
                    if (maxlen)
                        maxlen--;
                }
            }
            len++;
            if (outptr)
                *outptr++ = ']';
            if (maxlen)
                maxlen--;
        }

        len++; /* } */
        if (outptr)
            *outptr++ = '}';
        if (maxlen)
            maxlen--;
        pitor = ps_alignment_iter_next(pitor);
        if (pitor != NULL) {
            len++;
            if (outptr)
                *outptr++ = ',';
            if (maxlen)
                maxlen--;
        }
    }

    len += 2;
    if (outptr) {
        *outptr++ = ']';
        *outptr++ = '}';
        *outptr = '\0';
    }
    if (maxlen)
        maxlen--;

    return len;
}


int SphinxObject::format_align_iter(char *outptr, int maxlen,
                  ps_alignment_iter_t *itor, double utt_start, int frate, logmath_t *lmath)
{
    int start, duration, score;
    double prob, st, dur;
    const char *word;

    score = ps_alignment_iter_seg(itor, &start, &duration);
    st = utt_start + (double)start / frate;
    dur = (double)duration / frate;
    prob = logmath_exp(lmath, score);
    word = ps_alignment_iter_name(itor);
    if (word == NULL)
        word = "";

    return snprintf(outptr, maxlen, HYP_FORMAT, st, dur, prob, word);
}


int SphinxObject::format_seg(char *outptr, int len, ps_seg_t *seg,
           double utt_start, int frate,
           logmath_t *lmath)
{
    double prob, st, dur;
    int sf, ef;
    const char *word;

    ps_seg_frames(seg, &sf, &ef);
    st = utt_start + (double)sf / frate;
    dur = (double)(ef + 1 - sf) / frate;
    word = ps_seg_word(seg);
    if (word == NULL)
        word = "";
    prob = logmath_exp(lmath, ps_seg_prob(seg, NULL, NULL, NULL));
    len = snprintf(outptr, len, HYP_FORMAT, st, dur, prob, word);
    if (outptr) {
        outptr += len;
        *outptr++ = '}';
        *outptr = '\0';
    }
    len++;
    return len;
}



