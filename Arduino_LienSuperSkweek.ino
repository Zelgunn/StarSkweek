/* 2 à 5  => Fleches directionnelles
 * 6      => Bouton de tir
 * 7      => Bouton de pouvoir
 *
**/

int powerFreq = 0;

void setup() {
  Serial.begin(9600);
  for(char i = 2; i <= 7; i++)
    pinMode(i, INPUT);
}

void loop()
{
  // Fleches directionnelles
  int i;
  char dirs[4] = {'r','u','l','d'}; // Right/Up/Left/Down
  for(i = 0; i < 4; i++)
  {
    if(digitalRead(2+i))
    {
      Serial.print(dirs[i]);
      break;
    }
  }

  // Bouton de tir
  if(digitalRead(6))
  {
    Serial.print('s');
  }

  // Bouton de Pouvoir
  if(digitalRead(7))
  {
    Serial.print('p');
  }

  // Bouton de pouvoir
  if(powerFreq == 0)
  {
    int lightValue = analogRead(A0);
    Serial.print(',');
    Serial.print(lightValue, DEC);
  }
  powerFreq = (powerFreq + 1)%10;
}
