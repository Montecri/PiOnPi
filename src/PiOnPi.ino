/*
 * Computation of the n'th decimal digit of \pi with very little memory.
 * Written by Fabrice Bellard on January 8, 1997.
 *
 * We use a slightly modified version of the method described by Simon
 * Plouffe in "On the Computation of the n'th decimal digit of various
 * transcendental numbers" (November 1996). We have modified the algorithm
 * to get a running time of O(n^2) instead of O(n^3log(n)^3).
 *
 * This program uses mostly integer arithmetic. It may be slow on some
 * hardwares where integer multiplications and divisons must be done
 * by software. We have supposed that 'int' has a size of 32 bits. If
 * your compiler supports 'long long' integers of 64 bits, you may use
 * the integer version of 'mul_mod' (see HAS_LONG_LONG).
 */
/*
 * Changed by Cristiano Monteiro <cristianomonteiro@gmail.com> for the
 * PiOnPi calculating machine, based on Raspberry Pi Pico
 * There's room for improvement, feel free to implement.
 * March/2023
 */

#include <Arduino.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
/* Include DigitLedDisplay Library */
#include "DigitLedDisplay.h"

/* Arduino Pin to Display Pin
   7 to DIN,
   6 to CS,
   5 to CLK */
// # SPI SCK pin is GP18 (pin 24); SPI MOSI pin is GP19 (pin 25)
// # CS pin is GP17 (pin 22)
// DigitLedDisplay ld = DigitLedDisplay(25, 22, 24);
DigitLedDisplay ld = DigitLedDisplay(19, 17, 18);

/* uncomment the following line to use 'long long' integers */
#define HAS_LONG_LONG

#ifdef HAS_LONG_LONG
#define mul_mod(a, b, m) (((long long)(a) * (long long)(b)) % (m))
#else
#define mul_mod(a, b, m) fmod((double)a *(double)b, m)
#endif

unsigned long x = 1, StartTime, CurrentTime, ElapsedTime;
String PiReceived, PiDigits = "00000003";

const static byte digitos[] PROGMEM = { B01111110, B00110000, B01101101, B01111001, B00110011, B01011011, B01011111, B01110000, B01111111, B01111011 };
const static byte digitosponto[] PROGMEM = { B11111110, B10110000, B11101101, B11111001, B10110011, B11011011, B11011111, B11110000, B11111111, B11111011 };

/* return the inverse of x mod y */
int inv_mod(int x, int y) {
  int q, u, v, a, c, t;

  u = x;
  v = y;
  c = 1;
  a = 0;
  do {
    q = v / u;

    t = c;
    c = a - q * c;
    a = t;

    t = u;
    u = v - q * u;
    v = t;
  } while (u != 0);
  a = a % y;
  if (a < 0)
    a = y + a;
  return a;
}

/* return (a^b) mod m */
int pow_mod(int a, int b, int m) {
  int r, aa;

  r = 1;
  aa = a;
  while (1) {
    if (b & 1)
      r = mul_mod(r, aa, m);
    b = b >> 1;
    if (b == 0)
      break;
    aa = mul_mod(aa, aa, m);
  }
  return r;
}

/* return true if n is prime */
int is_prime(int n) {
  int r, i;
  if ((n % 2) == 0)
    return 0;

  r = (int)(sqrt(n));
  for (i = 3; i <= r; i += 2)
    if ((n % i) == 0)
      return 0;
  return 1;
}

/* return the prime number immediatly after n */
int next_prime(int n) {
  do {
    n++;
  } while (!is_prime(n));
  return n;
}

int calc(unsigned long z) {
  unsigned long n;
  int av, a, vmax, N, num, den, k, kq, kq2, t, v, s, i;
  double sum;

  //   if (argc < 2 || (n = atoi(argv[1])) <= 0) {
  // printf("This program computes the n'th decimal digit of \\pi\n"
  //        "usage: pi n , where n is the digit you want\n");
  // exit(1);
  //   }
  n = z;

  N = (int)((n + 20) * log(10) / log(2));

  sum = 0;

  for (a = 3; a <= (2 * N); a = next_prime(a)) {

    vmax = (int)(log(2 * N) / log(a));
    av = 1;
    for (i = 0; i < vmax; i++)
      av = av * a;

    s = 0;
    num = 1;
    den = 1;
    v = 0;
    kq = 1;
    kq2 = 1;

    for (k = 1; k <= N; k++) {

      t = k;
      if (kq >= a) {
        do {
          t = t / a;
          v--;
        } while ((t % a) == 0);
        kq = 0;
      }
      kq++;
      num = mul_mod(num, t, av);

      t = (2 * k - 1);
      if (kq2 >= a) {
        if (kq2 == a) {
          do {
            t = t / a;
            v++;
          } while ((t % a) == 0);
        }
        kq2 -= a;
      }
      den = mul_mod(den, t, av);
      kq2 += 2;

      if (v > 0) {
        t = inv_mod(den, av);
        t = mul_mod(t, num, av);
        t = mul_mod(t, k, av);
        for (i = v; i < vmax; i++)
          t = mul_mod(t, a, av);
        s += t;
        if (s >= av)
          s -= av;
      }
    }

    t = pow_mod(10, n - 1, av);
    s = mul_mod(s, t, av);
    sum = fmod(sum + (double)s / (double)av, 1.0);
  }
  // printf("Decimal digits of pi at position %d: %09d\n", n,
  //  (int) (sum * 1e9));
  return (sum * 1e9);
}

// Running on core1
void setup1() {
  //Serial.begin(9600);
  //delay(2000);
  // Serial.printf("C1: Red leader standing by...\n");

  // put your setup code here, to run once:
  /* Set the brightness min:1, max:15 */
  ld.setBright(10);

  /* Set the digit count */
  ld.setDigitLimit(8);

  // Write name
  //ld.write(8, B01110110);
  ld.write(8, B00001000);
  ld.write(7, B01100111);
  ld.write(6, B00110000);
  ld.write(5, B01001110);
  ld.write(4, B01110111);
  ld.write(3, B00001110);
  ld.write(2, B01001110);
  ld.write(1, B00001000);
  delay(4000);

  StartTime = millis();
  // ld.printDigit(12345678);
}

void loop1() {
  // Serial.printf("C1: Stay on target...\n");
  // delay(500);

  int z;
  uint32_t y = 0;
  byte dig;

  CurrentTime = millis();
  ElapsedTime = CurrentTime - StartTime;
  //Serial.println("PiDigits: " + PiDigits);

  if (PiDigits.length() < 9) {
    if (rp2040.fifo.pop_nb(&y)) {
      PiReceived = String(y);
      //Serial.println("y: " + String(y));
      //Serial.println("PiReceived: " + PiReceived);

      // If first character(s) is/are 0, they will be dropped from int variable
      // need to add them back. - Cristiano Monteiro - 20230313
      while (PiReceived.length() < 9) {
        PiReceived = "0" + PiReceived;
        //Serial.println("PiReceived0: " + PiReceived);
      }

      PiReceived = PiReceived.substring(0, 8);

      PiDigits = PiDigits + PiReceived;
    }
  }

  String temp = PiDigits.substring(0, 8);
  //Serial.println("temp: " + temp);

  for (z = 0; z <= 8; z++) {
    dig = pgm_read_byte_near(digitos + (temp[z] - '0'));
    ld.write(8 - z, dig);
  }

  if (ElapsedTime < 300) {
    // The bit manipulation tricks I tried to make the dot blink resulted in artifacts (other segments flicking).
    // Had to go with another byte array. - Cristiano Monteiro - 20230311
    //bitSet(dig, 7);
    //dig ^= (1 << 7);
    //dig = dig | B10000000;
    dig = pgm_read_byte_near(digitosponto + (temp[7] - '0'));
    ld.write(1, dig);
  } else if (ElapsedTime > 1000) {
    StartTime = millis();
  }

  if (PiDigits.length() > 8) {
    PiDigits = PiDigits.substring(1, PiDigits.length());
  }
  //Serial.println("PiDigits: " + PiDigits);
  //Serial.println("===================================");
  // Some delay, so then human eye can follow - Cristiano Monteiro - 20230313
  delay(250);
  //delay(1000);
}

void setup() {
  //Serial.begin(9600);
  //delay(2000);
}

void loop() {
  uint32_t w = calc(x);
  // The algorithm generates blocks of 9 digits, picking 8 and discarding 9th,
  // just to be sure I have the right ones - Cristiano Monteiro - 20230313
  x = x + 8;
  rp2040.fifo.push(w);
}
