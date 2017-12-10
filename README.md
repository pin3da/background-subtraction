# background-subtraction

Implementation of a correntropy-based adaptive filter used as a background
subtractor.

## Compile & run

```
make
./main.out
```

## Examples

#### Highway

Input image (original):

![original](./img/bg1-original.png)

Least Mean Squares filter:

![LMS](./img/bg1-lms.png)

Correntropy-based filter:

![correntropy](./img/bg1-corr.png)


#### Videos with heavy camera jitter

Input image (original):

![original](./img/bg-jitter-original.png)

Least Mean Squares filter:

![LMS](./img/bg-jitter-lms.png)

Correntropy-based filter:

![correntropy](./img/bg-jitter-corr.png)

## Datasets

The datasets can be downloaded from http://www.changedetection.net/

## Reference

This work is mainly based on the secction 2 of "Background modeling using
Object-based Selective Updating and Correntropy adaptation"

----

[Manuel Pineda](https://github.com/pin3da) - [Carlos Gonzalez](https://github.com/caal-15)
