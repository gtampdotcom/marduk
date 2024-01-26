![nabu psp 01](https://user-images.githubusercontent.com/910729/220287400-11513474-4032-421d-b3c8-b5bef1dfd7e4.jpg)
![nabu psp 02](https://user-images.githubusercontent.com/910729/220287409-68b1b30b-e032-42bf-8a9c-21dc35f346ec.jpg)
![nabu psp 04](https://user-images.githubusercontent.com/910729/220287412-2188747e-8efa-4e5f-bd57-7a0293bbbc5f.jpg)
![nabu psp 05](https://user-images.githubusercontent.com/910729/220287418-41b82c78-f0ae-44e6-9a11-d305775135a8.jpg)

This Sony PSP port of Marduk is just in very early stages. It runs slower than a real NABU PC and there's no virtual keyboard.

The IP address for the adapter is read from marduk.ini

What is Marduk?
===============

  Marduk is an attempt to emulate the obscure Canadian NABU Personal Computer.
  The code is in a very early state and very little works yet, but it's off to
  a pretty good start.

What is NABU?
=============

  NABU was both a company, and the name of the computer they are best known
  for.

  In the early 1980s, they created a computer that would interface with the
  local cable television service to download content rather than using local
  storage - an idea that was radical for the time, and only in very recent
  years becoming common.

  The computer, and the service that it interfaced with, were available in a
  couple cities in Canada, and apparently also had a small rollout in one
  Japanese location, but was generally unsuccessful.

How did this become a thing?
============================

  In late 2022, one of these computers appeared on the YouTube channel
  "Adrian's Digital Basement", giving it a boost of notoriety that led to many
  people finding out about it for the first time, including myself.  Shortly
  thereafter, the protocols were cracked and the computer was brought to life.

  Realizing that the main hardware of the NABU was all stuff I had familiarity
  with, I decided to try writing an emulator for it.

Status
======

  The CPU, VDP and PSG are emulated via third-party code, which I have
  imported with minimal adaptation.  Also, libsdl2 is used for the front end
  I/O code.

  CPU - Tested, working.
  
  VDP - Tested, working.
  
  PSG - Tested, working.
  
  Console lights - Tested, working. (not on the PSP port)
  
  Keyboard - Tested, working. (not on the PSP port)
  
  Joystick - Implemented but a little buggy on PSP
  
  Cable modem - Working, more or less.
  
  Strict speed control - Tested (mostly with another emulator), working. (PSP port is strictly slow)

Key bindings
============

  START = reset

  SELECT = ESC

  SQUARE = YES

  TRIANGLE = NO

  CIRCLE = space

  CROSS = GO (joystick button)

  dpad and thumbstick both send joystick movement

License
=======

  Marduk is released under the terms commonly known as the "MIT" license; you
  will find them attached to every source file as well as in "license.txt".

  Basically, give credit where credit's due.  It's a little more technical
  than that though.

  (Note that SDL uses a different license, but with the same goals.)
