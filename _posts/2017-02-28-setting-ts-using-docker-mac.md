---
layout: post
title: Setting tensorflow using docker on mac OS
tags: [linux]
---

## Environment Setting
[1. Install docker for mac.][1]  
2. Pull tensorflow image.
  ```
  $ docker pull tensorflow/tensorflow
  ```
[3-1. Launch a docker container that contains one of the tensorflow images.][2]
  ```
  $ docker run -it tensorflow/tensorflow bash
  ```
3-2. Test tensorflow sample code.
  ```
  $ python
  …
  >>> import tensorflow as tf
  >>> hello = tf.constant(‘Hello, TensorFlow!’)
  >>> sess = tf.Session()
  >>> print sess.run(hello)
  Hello, TensorFlow!
  >>>
  ```

## Coding with docker container
1. Code and test on docker.  
  ```
  $ docker cp sample.py ts-guide:/sample
  ```
2. Exit docker.
3. Save docker image.  
  ```
  $ docker ps -a  
  $ docker commit b4e3ed4abdc9 ts-guide  
  ```
4. Confirm saved docker image as browser
> Copy/paste this URL into your browser when you connect for the first time,  
> to login with a token:  
> http://localhost:8888/?token=f7af48d3d0078a1a09edda55533197d44baabe8011b89a79  


[1]:https://docs.docker.com/docker-for-mac/install
[2]:https://www.tensorflow.org/install/install_mac#ValidateInstallation
