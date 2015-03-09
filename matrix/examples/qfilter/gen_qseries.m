#!/usr/bin/octave -qf

## Octave/Matlab script to generate q quaternion time series with random
## noise on the rotation direction.

## Sample size.
n = 100;

## Amount of rotation around Euler axis per step.
arg = pi/20;
argRand = arg;
## argRand = 0;                   # For making unperturbed reference series.

## Basic unperturbed rotation per step.
vecRot = [0; -0.5; 1];
vecRot = vecRot/norm(vecRot);
qtnRot = quaternion(vecRot, arg);

## Initial attitude = straight and level.
qtn = quaternion(0, 0, 0, 1);
printf("%8.3f %8.3f %8.3f %8.3f %8.3f\n", i, qtn);

for i = 1:n
  ## Unperturbed rotation.
  qtn = qmult(qtn, qtnRot);

  ## Apply random perturbation. NOTE: Mean of rand(1) is 0.5.
  vecRand = [rand(1) - 0.5; rand(1) - 0.5; rand(1) - 0.5];
  vecRand = vecRand/norm(vecRand);
  qtnRand = quaternion(vecRand, argRand*rand(1));
  printf("%8.3f %8.3f %8.3f %8.3f %8.3f\n", i, qmult(qtn, qtnRand));
endfor
