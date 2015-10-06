/*
 * Percentile Sampler
 */
#ifndef PERCENTILES_H
#define PERCENTILES_H

#include <stddef.h>

#define PERCENTILE_SAMPLE_COUNT 1024
typedef long long percentile_sample_t;

struct percentile_sampler {
    size_t total_items;
    percentile_sample_t samples[PERCENTILE_SAMPLE_COUNT];
};
typedef struct percentile_sampler percentile_sampler_t;

extern void percentile_sampler_init(percentile_sampler_t *sampler);
extern void percentile_sampler_reset(percentile_sampler_t *sampler);
extern void percentile_sampler_record_sample(percentile_sampler_t *sampler,
                                             percentile_sample_t item);
extern void percentile_sampler_snapshot(const percentile_sampler_t* sampler,
                                        percentile_sample_t *results,
                                        const double *percentiles,
                                        size_t precentiles_length);

#endif /* PERCENTILES_H */
