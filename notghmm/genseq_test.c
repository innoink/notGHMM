// genseq_test.c
// Test the generate sequence function.
//
// Author : Weipeng He <heweipeng@gmail.com>
// Copyright (c) 2014, All rights reserved.

#include "notghmm.h"

#include <stdio.h>
#include <math.h>

int main(int argc, char** argv) {
  gsl_rng_env_setup();
  gsl_rng* rng = gsl_rng_alloc(gsl_rng_default);

  hmmgmm_t* model = hmmgmm_alloc(2, 1, 2, 0);

  gsl_vector_set(model->pi, 0, 1.0);
  gsl_vector_set(model->pi, 1, 0.0);

  gsl_matrix_set(model->a, 0, 0, .3);
  gsl_matrix_set(model->a, 0, 1, .7);
  gsl_matrix_set(model->a, 1, 0, .8);
  gsl_matrix_set(model->a, 1, 1, .2);

  gsl_vector_set(model->states[0]->weight, 0, 1.0);
  gsl_vector_set(model->states[1]->weight, 0, 1.0);

  gmm_t* state = model->states[0];
  gsl_vector_set(state->comp[0]->mean, 0, 0.0);
  gsl_vector_set(state->comp[0]->mean, 1, 0.0);
  gsl_matrix_set(state->comp[0]->cov, 0, 0, 1.0);
  gsl_matrix_set(state->comp[0]->cov, 0, 1, 0.0);
  gsl_matrix_set(state->comp[0]->cov, 1, 0, 0.0);
  gsl_matrix_set(state->comp[0]->cov, 1, 1, 1.0);

  state = model->states[1];
  gsl_vector_set(state->comp[0]->mean, 0, 1.0);
  gsl_vector_set(state->comp[0]->mean, 1, 1.0);
  gsl_matrix_set(state->comp[0]->cov, 0, 0, 1.0);
  gsl_matrix_set(state->comp[0]->cov, 0, 1, -1.0);
  gsl_matrix_set(state->comp[0]->cov, 1, 0, -1.0);
  gsl_matrix_set(state->comp[0]->cov, 1, 1, 2.0);

  hmmgmm_t* model2 = hmmgmm_alloc(2, 1, 2, 0);

  gsl_vector_set(model2->pi, 0, .5);
  gsl_vector_set(model2->pi, 1, .5);

  gsl_matrix_set(model2->a, 0, 0, .9);
  gsl_matrix_set(model2->a, 0, 1, .1);
  gsl_matrix_set(model2->a, 1, 0, .3);
  gsl_matrix_set(model2->a, 1, 1, .7);

  gsl_vector_set(model2->states[0]->weight, 0, 1.0);
  gsl_vector_set(model2->states[1]->weight, 0, 1.0);

  state = model2->states[0];
  gsl_vector_set(state->comp[0]->mean, 0, 1.0);
  gsl_vector_set(state->comp[0]->mean, 1, 1.0);
  gsl_matrix_set(state->comp[0]->cov, 0, 0, 1.0);
  gsl_matrix_set(state->comp[0]->cov, 0, 1, 0.0);
  gsl_matrix_set(state->comp[0]->cov, 1, 0, 0.0);
  gsl_matrix_set(state->comp[0]->cov, 1, 1, 1.0);

  state = model2->states[1];
  gsl_vector_set(state->comp[0]->mean, 0, 0.0);
  gsl_vector_set(state->comp[0]->mean, 1, 1.0);
  gsl_matrix_set(state->comp[0]->cov, 0, 0, 2.0);
  gsl_matrix_set(state->comp[0]->cov, 0, 1, 0.0);
  gsl_matrix_set(state->comp[0]->cov, 1, 0, 0.0);
  gsl_matrix_set(state->comp[0]->cov, 1, 1, 1.0);

  hmmgmm_t* model3 = hmmgmm_alloc(2, 1, 2, 1);

  gsl_vector_set(model3->pi, 0, .5);
  gsl_vector_set(model3->pi, 1, .5);

  gsl_matrix_set(model3->a, 0, 0, .9);
  gsl_matrix_set(model3->a, 0, 1, .1);
  gsl_matrix_set(model3->a, 1, 0, .3);
  gsl_matrix_set(model3->a, 1, 1, .7);

  gsl_vector_set(model3->states[0]->weight, 0, 1.0);
  gsl_vector_set(model3->states[1]->weight, 0, 1.0);

  state = model3->states[0];
  gsl_vector_set(state->comp[0]->mean, 0, 1.0);
  gsl_vector_set(state->comp[0]->mean, 1, 1.0);
  gsl_vector_set(state->comp[0]->diag, 0, 1.0);
  gsl_vector_set(state->comp[0]->diag, 1, 1.0);

  state = model3->states[1];
  gsl_vector_set(state->comp[0]->mean, 0, 0.0);
  gsl_vector_set(state->comp[0]->mean, 1, 1.0);
  gsl_vector_set(state->comp[0]->diag, 0, 2.0);
  gsl_vector_set(state->comp[0]->diag, 1, 1.0);

  size_t size = 1000;
  seq_t* seq = seq_gen(model, size, rng);

  gsl_matrix* logalpha = gsl_matrix_alloc(size, model->n);
  forward_proc_log(model, seq, logalpha);

  gsl_matrix* logalpha2 = gsl_matrix_alloc(size, model2->n);
  forward_proc_log(model2, seq, logalpha2);

  gsl_matrix* logalpha3 = gsl_matrix_alloc(size, model3->n);
  forward_proc_log(model3, seq, logalpha3);

  gsl_matrix* logbeta = gsl_matrix_alloc(size, model->n);
  backward_proc_log(model, seq, logbeta);

  size_t* hidden = calloc(size, sizeof(size_t));
  double p = viterbi_log(model, seq, hidden);

  size_t i;
  for (i = 0; i < size; i++) {
    printf("%g %g; %g %g; %g %g; %g %g; %g %g\n",
        gsl_vector_get(seq->data[i], 0),
        gsl_vector_get(seq->data[i], 1),
        gsl_matrix_get(logalpha, i, 0),
        gsl_matrix_get(logalpha, i, 1),
        gsl_matrix_get(logalpha2, i, 0),
        gsl_matrix_get(logalpha2, i, 1),
        gsl_matrix_get(logalpha3, i, 0),
        gsl_matrix_get(logalpha3, i, 1),
        gsl_matrix_get(logbeta, i, 0),
        gsl_matrix_get(logbeta, i, 1));
  }

  printf("================\n");
  gsl_matrix_add(logbeta, logalpha);
  for (i = 0; i < size; i++) {
    gsl_vector_view v = gsl_matrix_row(logbeta, i);
    printf("%g %g; %g\n",
        gsl_matrix_get(logbeta, i, 0),
        gsl_matrix_get(logbeta, i, 1),
        log_sum_exp(&v.vector));
  }

  printf("================\n");
  printf("max p = %g\n", p);
  for (i = 0; i < size; i++) {
    printf("%g %g; %ld\n",
        gsl_vector_get(seq->data[i], 0),
        gsl_vector_get(seq->data[i], 1),
        hidden[i]);
  }

  printf("\n================\nModel 1\n");
  hmmgmm_fprint(stdout, model);
  printf("\n================\nModel 2\n");
  hmmgmm_fprint(stdout, model2);
  printf("\n================\nModel 3\n");
  hmmgmm_fprint(stdout, model3);


  size_t nos = 100;
  seq_t** data = calloc(nos, sizeof(seq_t*));
  for (i = 0; i < nos; i++) {
    data[i] = seq_gen(model, size, rng);
  }
  
  double po = 0.0;
  double pn = 0.0;
  double p3 = 0.0;
  for (i = 0; i < nos; i++) {
    forward_proc_log(model, data[i], logalpha);
    po += hmm_log_likelihood(logalpha);
    forward_proc_log(model2, data[i], logalpha);
    pn += hmm_log_likelihood(logalpha);
    forward_proc_log(model3, data[i], logalpha);
    p3 += hmm_log_likelihood(logalpha);
  }

  baum_welch(model2, data, nos);

  printf("\n================\nModel 2 (re-estimated)\n");
  hmmgmm_fprint(stdout, model2);

  baum_welch(model3, data, nos);

  printf("\n================\nModel 3 (re-estimated)\n");
  hmmgmm_fprint(stdout, model3);

  double pr = 0.0;
  double p3r = 0.0;
  for (i = 0; i < nos; i++) {
    forward_proc_log(model2, data[i], logalpha);
    pr += hmm_log_likelihood(logalpha);
    forward_proc_log(model3, data[i], logalpha);
    p3r += hmm_log_likelihood(logalpha);
  }

  printf("\n P = %g, (%g, %g) => (%g, %g)\n", po, pn, p3, pr, p3r);

  hmmgmm_free(model);
  hmmgmm_free(model2);
  hmmgmm_free(model3);
  seq_free(seq);
  for (i = 0; i < nos; i++) {
    seq_free(data[i]);
  }
  free(data);
  gsl_matrix_free(logalpha);
  gsl_matrix_free(logalpha2);
  gsl_matrix_free(logalpha3);
  gsl_matrix_free(logbeta);
  free(hidden);
  gsl_rng_free(rng);

  return 0;
}

