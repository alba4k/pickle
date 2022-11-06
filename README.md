# Pickle Encoding

## This encoding was created by [Quads#2864](https://discord.com/users/427095706000556044), **all credit goes to him**. All I did was creating this implementation and slightly modifying the encoding (this version of the encoding can be called 8-bit pickle).
## All I did was slightly adjust it (as described in this file and in [src/pickle.c](https://github.com/alba4k/pickle/blob/master/src/pickle.c))

---

![pickle](img/pickle.png)

### Have you ever wished that you could transform everything in pickles? Well, now you can. Introducing: Pickle Encoding! 🥒🥒🥒


# Installation
## So, since you obviously want this on your machine too, here are the steps needed to get this installed (Linux and macOS)
```shell
$ git clone https://github.com/alba4k/pickle
$ cd pickle
# make install
```
You will need to run `make install` as root (using sudo, doas, ...) to get access to /usr.
Please note that clang is set as the default C compiler. Use `make CC=[compiler]` (e.g. `CC=gcc`) to use an other compiler.

# Building
## If you still feel the urge to have this, but just to try it, then you can just compile it and run it (Linux, macOS, Windows, BSD, probably TempleOS too)
```shell
$ git clone https://github.com/alba4k/pickle
$ cd pickle
$ make
```
clang is set as the default C compiler. Use `make CC=[compiler]` (e.g. `CC=gcc`) to use an other compiler.

# How does this work?
### ⚠️ Please note that I am assuming that the reader understands how binary, hexadecimal and ASCII work.
## The encoding process
This encoding transforms any sort of text (UTF-8 included) into a pickly mess. This process is, however, completely predictable and reversible. This is how it goes.

First of all, the encoder will transform every byte of text into two "Pickle Words". A pickle word is a string of three characters that can be either "PIC" or "KLE", with any combination of lower and uppercase characters.

The conversion is pretty easy. We start by looking at the UTF-8 (we will be looking at ASCII examples for simplicity) representation of every character. Let's assume we have the following string:

```
Hello
```

Converting it to ASCII would go like this:

```
H  ->  0x48  ->  0b01001000
e  ->  0x65  ->  0b01100101
l  ->  0x6C  ->  0b01101100
l  ->  0x6C  ->  0b01101100
o  ->  0x6F  ->  0b01101111
```

I will mostly refer to hexadecimal value as they are overall easier to understand.

Now, we can see how every character is made of 8 bits. We now need to somehow convert this to pickle words. The first step will be to divide these bytes into groups of 4 bits (e.g. 0x48 -> 0x4; 0x8).

Every 4-bit group will become a pickle word. 

Now, we need to determine if a group is going to become a "PIC" or a "KLE". This is done by looking at the last bit of each group. In this implementation, even groups (those ending with a 0) will become "PIC" words and odd ones (those ending with a 1) will become a "KLE".

Let's see how this affects our "Hello":

```
0x48  ->  0x4  ->  PIC
      ->  0x8  ->  PIC
0x65  ->  0x6  ->  PIC
      ->  0x5  ->  KLE
0x6C  ->  0x6  ->  PIC
      ->  0xC  ->  PIC
0x6C  ->  0x6  ->  PIC
      ->  0xC  ->  PIC
0x6F  ->  0x6  ->  PIC
      ->  0xF  ->  KLE
```

We have now used the last bit of the 4 that make up each group.

We will use the remaining ones to determine which letters of the pickle word will be uppercase. This is done by looking at the first three bits of a group.

The 3 remaining bits correspong to the characters in out pickle word. 1 will give us uppercase, 0 will give us lowercase. In our example, this would look like this.

```
0x48  ->  0x4  ->  pIc
      ->  0x8  ->  Pic
0x65  ->  0x6  ->  pIC
      ->  0x5  ->  kLe
0x6C  ->  0x6  ->  pIC
      ->  0xC  ->  PIc
0x6C  ->  0x6  ->  pIC
      ->  0xC  ->  PIc
0x6F  ->  0x6  ->  pIC
      ->  0xF  ->  KLE
```

And we are now basically done. All that is left is to concatenate these pickle words all together:

```
pIC Pic pIC kLe pIC PIc pIC PIc pIC KLE  ->  pICPicpICkLepICPIcpICPIcpICKLE
```

In a nutshell:

```
Hello
H         e         l         l         o
0x48       0x65     0x6C      0x6C      0x6F
0x4  0x8  0x6  0x5  0x6  0xC  0x6  0xC  0x6  0xF
pIc  Pic  pIC  kLe  pIC  PIc  pIC  PIc  pIC  KLE
pIcPicpICkLepICPIcpICPIcpICKLE
```

Since it might be useful for a fast conversion, here is a look-up table for hexadecimal values:

Value | Word |  \|  | Value | Word
--- | --- | --- | --- | ---
0x0 | pic |  \|  | 0x8 | Pic
0x1 | kle |  \|  | 0x9 | Kle
0x2 | piC |  \|  | 0xA | PiC
0x3 | klE |  \|  | 0xB | KlE
0x4 | pIc |  \|  | 0xC | PIc
0x5 | kLe |  \|  | 0xD | KLe
0x6 | pIC |  \|  | 0xE | PIC
0x7 | kLE |  \|  | 0xF | KLE

Also, note that encoding using this method will increase the size of the text by 6 times, since every byte will become 6B.

## The decoding process
Decoding a pickle-encoded string will include the same operations made to encode it, in the opposite order.

Let's use as an example the same string used in the encoding process:

```
pICPicpICkLepICPIcpICPIcpICKLE
```

The first thing we want to do is deconcatenating the single pickle words from our pickle string, and we know that a pickle word is always three characters long. This means:


```
pICPicpICkLepICPIcpICPIcpICKLE  ->  pIC Pic pIC kLe pIC PIc pIC PIc pIC KLE
```

Now, we want to convert every pickle word to the corresponding hexadecimal value. To do so we should first look at the type of the word and determine the last bit of each 4-bit group (a "PIC" corresponds to a \*\*\*0 group, a "KLE" to a \*\*\*1), and then go back to the exact group using the capitalization to get the remaining 3 bits.

Or we can cheat. We've already come up with a look-up table, so why not just use that one:

```
pIC  ->  0x4
Pic  ->  0x8
pIC  ->  0x6
kLe  ->  0x5
pIC  ->  0x6
PIc  ->  0xC
pIC  ->  0x6
PIc  ->  0xC
pIC  ->  0x6
KLE  ->  0xF
```

And now we need to combine our 4-bit groups back into full bytes, in our case:

```
0x4 ~ 0x8  ->  0x48
0x6 ~ 0x5  ->  0x65
0x6 ~ 0xC  ->  0x6C
0x6 ~ 0xC  ->  0x6C
0x6 ~ 0xF  ->  0x6F
```

And now all that's left to do is to convert those hexadecimal values:

```
0x48  ->  H
0x65  ->  e
0x6C  ->  l
0x6c  ->  l
0x6F  ->  o
```

And with this last step we get:

```
Hello
```

So basically, what we did was:

```
pIcPicpICkLepICPIcpICPIcpICKLE
pIc  Pic  pIC  kLe  pIC  PIc  pIC  PIc  pIC  KLE
0x4  0x8  0x6  0x5  0x6  0xC  0x6  0xC  0x6  0xF
0x48      0x65      0x6C      0x6C      0x6F
H         e         l         l         o
Hello
```

## Differences with the original encoding
This differs in some way by the encoding initially developed by Quads. There are some differences:

### 1) I'm considering 8 bit characters
The original pickle encoding considered 8 bits for every characters, it excluded the sign bit. This means that we would not have obtained 2 pickle words from 1 byte of text, we would instead have obtained 7 pickle words from 4 bytes of text (a 5.25x size increase)

This is what that would look like (Consider a 0b before every number):
```
Hello
H          e          l          l          o
1001000    1100101    1101100    1101100    1101111
1001  0001  1001  0111  0110  0110  1100  1101  111
```
Now, we can already see one of the reasons why I chose to consider 8 bits for every character. We eliminate the need for padding. Since we read the pickle words in the opposite order (more on this later, pt.3), we will place a 0 at the start of the first word:
```
0100  1000  1100  1011  1011  0011  0110  0110  1111
```

There are a few other reasons why I chose to use 8 bits per character:
* speed: characters are saved using 8 bits in memory, which means that I could just reuse the characters saved in RAM and I would need way less processing (e.g. no need for padding).
* With 8 bits every character becomes 2 pickle words, which is way easier to implement than 4 characters becoming 7 pickle words.
* It makes unicode handling way easier, since I don't need escape sequences to represent UTF-8 characters

_**Since this is the biggest change and the one that caused all the others, this version can be known as 8-bit pickle encoding**_

### 2) I'm using the last bit of the group to determine the word
The original pickle encoding used the first bit of every byte to determine the pickle word, and the last 3 for capitalization. In our previous example:

```
0100  1000  1100  1011  1011  0011  0110  0110  1111
Pic   Kle   KLe   kLE   kLE   pIC   PIc   PIc   KLE
```

I would have ended up with way more "PIC" than "KLE", since the sign bit of every ASCII character is always 0.

The original encoding avoided this issue by using 7 bits. This made multiple characters combine in a series of pickle words, therefore randomizing the position of each bit.

### 3) I'm not reversing the order of the pickle words.
The original pickle encoding added an extra step at the end of the encoding, it would reverse the order of the pickle words. In our previous example:

```
Pic  Kle  KLe  kLE  kLE  pIC  PIc  PIc  KLE
```

Would have become:

```
KLE  PIc  PIc  pIC  kLE  kLE  KLe  Kle  Pic
```

I chose to remove this inversion because of speed and since it made everything harder to encode and decode manually. 

---

## _This whole document is useless, I don't even know why I put so much effort into documenting this codec. I don't think I will document the library as deepy, so read the code. 🥒🥒_

---

### ©️ Aaron Blasko, November 2022