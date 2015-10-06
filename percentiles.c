#include "percentiles.h"
#include <stdlib.h>
#include <string.h>

static int compare_samples(const void *a, const void *b) {
    const percentile_sample_t *sample_a = (const percentile_sample_t*) a;
    const percentile_sample_t *sample_b = (const percentile_sample_t*) b;
    return (*sample_a > *sample_b) - (*sample_a < *sample_b);
}

void percentile_sampler_init(percentile_sampler_t *sampler) {
    percentile_sampler_reset(sampler);
}

void percentile_sampler_reset(percentile_sampler_t *sampler) {
    memset(sampler, 0, sizeof(*sampler));
}

void percentile_sampler_record_sample(percentile_sampler_t *sampler,
                                      percentile_sample_t item) {
    if (sampler->total_items < PERCENTILE_SAMPLE_COUNT) {
        sampler->samples[sampler->total_items] = item;
    } else {
        long r = random() % sampler->total_items;
        if (r < PERCENTILE_SAMPLE_COUNT) {
            sampler->samples[r] = item;
        }
    }

    sampler->total_items += 1;
}

void percentile_sampler_snapshot(
    const percentile_sampler_t* sampler, percentile_sample_t *results,
    const double *percentiles, size_t percentiles_length) {

    if (sampler->total_items == 0) {
        memset(results, 0, sizeof(*results) * percentiles_length);
    } else {
        // sort samples
        size_t samples_count = sampler->total_items < PERCENTILE_SAMPLE_COUNT ?
            sampler->total_items : PERCENTILE_SAMPLE_COUNT;
        percentile_sample_t samples[PERCENTILE_SAMPLE_COUNT];
        size_t i;

        memcpy(samples, sampler->samples, sizeof(*samples) * samples_count);
        qsort(samples, samples_count, sizeof(*samples), compare_samples);

        for (i = 0; i < percentiles_length; ++i) {
            results[i] = samples[(int)(samples_count * percentiles[i])];
        }
    }
}
