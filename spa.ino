char reshortn[30];
char directions[30];
char shortn[30];
char rev[30];
#include<LiquidCrystal.h>
LiquidCrystal lcd(12,11,5,4,3,2);    //lcd pins
int rm1=24;                          //motor pins
int rm2=25;
int lm1=0;
int lm2=1;
int l,c1,c2,c3,r,j;                    //sensor pins
int a = 0;
int i=0;
int tdelay=750;                      //turn delay
int fdelay=50;                        //forward delay
int k=0;
int n=0;
void Stop()                          //stop
{
  digitalWrite(lm1,0);
  digitalWrite(lm2,0);
  digitalWrite(rm1,0);
  digitalWrite(rm2,0);
}
void forward()                        //forward
{
  digitalWrite(lm1,1);
  digitalWrite(lm2,0);
  digitalWrite(rm1,1);
  digitalWrite(rm2,0);
} 
void smallright()                  //small right
{
  digitalWrite(lm1,0);
  digitalWrite(lm2,0);
  digitalWrite(rm1,0);
  digitalWrite(rm2,1);
}
void smallleft()                    //small left
{
  digitalWrite(lm1,0);
  digitalWrite(lm2,1);
  digitalWrite(rm1,0);
  digitalWrite(rm2,0);
}
void right()                        //right
{
  digitalWrite(lm1,1);
  digitalWrite(lm2,0);
  digitalWrite(rm1,0);
  digitalWrite(rm2,1);
}
void left()                          //left
{
  digitalWrite(lm1,0);
  digitalWrite(lm2,1);
  digitalWrite(rm1,1);
  digitalWrite(rm2,0);
}
int eosens()
{
  readsens();
  if(((c1+c2+c3)==1)||((c1+c2+c3)==2))
  return 1;
  else
  return 0;
}
void readsens()                      //reading sensor vALUES
{
  l=digitalRead(26);
  c1=digitalRead(27);
  c2=digitalRead(28);
  c3=digitalRead(29);
  r=digitalRead(30);
  lcd.print(l);
  lcd.print("--");
  lcd.print(c1);
  lcd.print("--");
  lcd.print(c2);
  lcd.print("--");
  lcd.print(c3);
  lcd.print("--");
  lcd.print(r);
  lcd.print("--");
  }
void inch()
{
  lcd.print("inch function");
  Stop();              //stop
  delay(500);
  forward();           //forward
  delay(450);
  Stop();
  delay(500);
  lcd.clear();
  readsens();          //read the calue of sensors
}
void align()            //align function
{
  Stop();              //stop
  delay(500);
  forward();            //forward
  delay(50);
  Stop();              //stop
  delay(500);
  lcd.clear();
  readsens();          //read the sensor values
}
void printing(char prtdirection[])    //function to print the values on the lcd
{
  lcd.setCursor(1,1);
  for(i=0;prtdirection[i]!='E';i++)
  {
    lcd.print(prtdirection[i]);
  }
  lcd.print('E');
  delay(1000);
}
void setup()
{
  lcd.begin(16,2);                    //initialize lcd
  lcd.print("come on boys");;
  delay(500);                          //clear lcd
  lcd.clear();
  pinMode(lm1,OUTPUT);                  //declare output pins
  pinMode(lm2,OUTPUT);
  pinMode(rm1,OUTPUT);
  pinMode(rm2,OUTPUT);
  pinMode(26,INPUT);                    //declare input pins
  pinMode(27,INPUT);
  pinMode(28,INPUT);
  pinMode(29,INPUT);
  pinMode(30,INPUT);
}
void loop()
{
  lcd.clear();                        //clear lcd
  readsens();                        //read sensor values
  lcd.setCursor(1,1);
  //LINE FOLLOWER LOGIC
  //Forward
  if((l==1)&&(c1==1)&&(c2==0)&&(c3==1)&&(r==1))
  {
    lcd.print("Forward");
    forward();
  }
  //Left
  else if(((l==1)&&(c1==0)&&(c2==1)&&(c3==1)&&(r==1)) || ((l==1)&&(c1==0)&&(c2==0)&&(c3==1)&&(r==1)))
  {
    lcd.print("Left");
    smallleft();
  }
  //Right
  else if(((l==1)&&(c1==1)&&(c2==1)&&(c3==0)&&(r==1)) || ((l==1)&&(c1==1)&&(c2==0)&&(c3==0)&&(r==1)))
  {
    lcd.print("Right");
    smallright();
  }
  //Dead End
  else if((l==1)&&(c1==1)&&(c2==1)&&(c3==1)&&(r==1))
  {
    lcd.print("U Turn");
    right();                //u turn
    delay(1400);            //delay for u turn
    directions[i]='U';
    i++;
  }
  
  //INCH LOGIC
  else if(((l==0)&&(c1==0)) || ((c3==0)&&(r==0)))
  {
    align();                  //align
    
    //RIGHT ONLY AND STRAIGHT AND RIGHT
    if(( (l==1)&&(c1==1)&&(c2==0)&&(c3==0)&&(r==0)) ||( (l==1)&&(c1==0)&&(c2==0)&&(c3==0)&&(r==0)))
    {
      lcd.print("Right or Straight and right?");
      inch();
      if((l==1)&&(c1==1)&&(c2==1)&&(c3==1)&&(r==1))
      {
        lcd.print("Right");
        right();              //right
        delay(tdelay);        //turn delay
      }
      else if((l==1)&&(r==1)&&(eosens()))
      {
        lcd.print("Straight");
        directions[i]='S';      //store S in the array
        i++;                    //increment to the next position in the array
        forward();              //forward
        delay(fdelay);          //forward delay
      }
    }
    
    //LEFT ONLY AND STRAIGHT LEFT
    else if(( (l==0)&&(c1==0)&&(c2==0)&&(c3==1)&&(r==1))||( (l==0)&&(c1==0)&&(c2==0)&&(c3==0)&&(r==1)))
    {
      lcd.print("Left or Straight and left");
      inch();
      if((l==1)&&(c1==1)&&(c2==1)&&(c3==1)&&(r==1))
      {
        lcd.print("Left");
        left();                 //left
        delay(tdelay);          //turn delay
      }
      else if((l==1)&&(r==1)&&(eosens()))
      {
        lcd.print("Left");
        directions[i]='L';        //store L in the array
        i++;                      //increment to the next position in the array
        left();                    //left
        delay(tdelay);            //turn delay
      }
    }
    
    //4-way/ T-int/ End of Maze
    else if((l==0)&&(c1==0)&&(c2==0)&&(c3==0)&&(r==0))
    {
      lcd.print("T/END/4");
      inch();
      if((l==1)&&(r==1)&&(eosens()))
      {
        lcd.print("4 way");
        directions[i]='L';        //store L in the array
        i++;                      //increment to the next position in the array
        left();                    //left
        delay(tdelay);            //turn delay
      }
      else if((l==1)&&(c1==1)&&(c2==1)&&(c3==1)&&(r==1))
      {
        lcd.print("T-int");
        directions[i]='L';        //store L in the array
        i++;                      //increment to the next position in the array
        left();                    //left
        delay(tdelay);            //turn delay
      }
      else if((l==0)&&(c1==0)&&(c2==0)&&(c3==0)&&(r==0))
     {
      lcd.print("End of Maze");
      directions[i]='E';            //store E into the array
              Stop();                    //stop
              
        delay(5000);
        printing(directions);          //printing the final path on the lc
        lcd.clear();
        replacement(directions);
     }
    }
  }
}
void replacement(char shortn[])            //replacement function
{
  lcd.print("Entered Replacement");
  delay(1000);
  lcd.clear();                          //clear lcd
  for(i=0;shortn[i]!='E';i++)            //read all the elements inn the array
  {
    if(shortn[i]=='U')                //check if the element is U
    {
      if((shortn[i-1]=='S')&&(shortn[i+1]=='L')) //SUL is replaced by XXR
      {
        shortn[i-1]='x';
        shortn[i]='x';
        shortn[i+1]='R';
      }
      else if((shortn[i-1]=='L')&&(shortn[i+1]=='L'))    //LUL is replaced by XXS
      {
        shortn[i-1]='x';
        shortn[i]='x';
        shortn[i+1]='S';
      }
     else if((shortn[i-1]=='L')&&(shortn[i+1]=='S'))      //LUS is replaced by XXR
      {
        shortn[i-1]='x';
        shortn[i]='x';
        shortn[i+1]='R';
      }
     else if((shortn[i-1]=='L')&&(shortn[i+1]=='R'))      //LUR is replaced by XXU
      {
        shortn[i-1]='x';
        shortn[i]='x';
        shortn[i+1]='U';
      }
     else if((shortn[i-1]=='R')&&(shortn[i+1]=='L'))    //RUL is replaced by XXU
      {
        shortn[i-1]='x';
        shortn[i]='x';
        shortn[i+1]='U';
      }
     else if((shortn[i-1]=='S')&&(shortn[i+1]=='S'))    //SUS is replaced by XXU
      {
        shortn[i-1]='x';
        shortn[i]='x';
        shortn[i+1]='U';
      }
    }
  }
  lcd.clear();                                        //clear LCD
  lcd.print("Finished new path:");
  delay(1000);
  lcd.clear();
  lcd.print("New:");
  printing(shortn);
  int j=0;
  for(i=0;shortn[i]!='E';i++)                        //read every element in the array
  {
    if(shortn[i]!='x')                              //check is the element is not X
    {
      reshortn[j]=shortn[i];                        //if it is not x copy it to the new array
      j++;
    }
  }
  reshortn[j]='E';
  lcd.setCursor(1,1);
  lcd.print("SPA=");
  printing(reshortn);                              //print the path in the lcd
  delay(1000);
  for(i=0;reshortn[i]!='E';i++)                  //read the new array
  {
    if(reshortn[i]=='U')                        //check if the element is U
    {
    replacement(reshortn);                      //implement the replacement rules again
    }
  }
  lcd.clear();                                //clear the LCD
  lcd.print("FINAL=");
  printing(reshortn);                        //print the shortest path
  lcd.clear();
  
  int n = strlen(reshortn);
  lcd.print("N =  ");
  lcd.print(n);
  delay(1000);
  int b = 0;
  lcd.clear();
  for(int z=n-2; z>=0; z--)
  {
    lcd.print(z);
    rev[b]=reshortn[z];
    b++;
  }
  rev[b]='E';
  delay(100);
  lcd.clear();
  int w=strlen(rev);
  lcd.print("rev length=  ");
  lcd.print(w);
  delay(1000);
  lcd.clear();
  lcd.print("Rev:");
  printing(rev);  
  final();   //call the final path
}
void decisions()
{
  lcd.print("Entered decisions");
//  lcd.clear();                            //clear the lcd
  if(reshortn[k]=='S')                    //if the element is S
  {
    forward();                            //forward
  //  delay(fdelay);                        //fdelay
  }
  if(reshortn[k]=='L')                  //if the element is L
  {
    left();                            //left
    delay(tdelay);                    //tdelay
  }
  if(reshortn[k]=='R')                  //if the element is R
  {
    right();                            //right
    delay(tdelay);                       //tdelay
  }
  k++;  //increment position in the array
}

//***********************************************************FINAL**************************************************************************************

void final()
{
  lcd.clear();                          //clear the lcd
  lcd.print("ENTERED FINAL");
  while(1)
  {
    lcd.clear();
   readsens();
   lcd.setCursor(1,1);
   //***************************************************************LINE FOLLOWER SENSORS************************************************************
   //Forward
  if((l==1)&&(c1==1)&&(c2==0)&&(c3==1)&&(r==1))
  {
    lcd.print("Forward");
    forward();
  }
  //Left
  else if(( (l==1)&&(c1==0)&&(c2==1)&&(c3==1)&&(r==1))||( (l==1)&&(c1==0)&&(c2==0)&&(c3==1)&&(r==1)))
  {
    lcd.print("Left");
    smallleft();
  }
  //Right
  else if(( (l==1)&&(c1==1)&&(c2==1)&&(c3==0)&&(r==1))||( (l==1)&&(c1==1)&&(c2==0)&&(c3==0)&&(r==1)))
  {
    lcd.print("Right");
    smallright();
  }
  
  //*******************************************MAZE 3 CATEGORIES*************************************************************************************
   else if(( (l==0)&&(c1==0))||((c3==0)&&(r==0)))
  {
    align();                  //align
    
    //RIGHT ONLY AND STRAIGHT AND RIGHT
    if(( (l==1)&&(c1==1)&&(c2==0)&&(c3==0)&&(r==0))||( (l==1)&&(c1==0)&&(c2==0)&&(c3==0)&&(r==0)))
    {
      lcd.print("Right or Straight and right?");
      inch();
      if((l==1)&&(c1==1)&&(c2==1)&&(c3==1)&&(r==1))
      {
        lcd.print("Right");
        right();              //right
        delay(tdelay);        //turn delay
      }
      else if((l==1)&&(r==1)&&(eosens()))
      {
        decisions();
      }
    }
  //LEFT ONLY AND STRAIGHT LEFT
    else if(( (l==0)&&(c1==0)&&(c2==0)&&(c3==1)&&(r==1))||( (l==0)&&(c1==0)&&(c2==0)&&(c3==0)&&(r==1)))
    {
      lcd.print("Left or Straight and left");
      inch();
      if((l==1)&&(c1==1)&&(c2==1)&&(c3==1)&&(r==1))
      {
        lcd.print("Left");
        left();                 //left
        delay(tdelay);          //turn delay
      }
      else if((l==1)&&(r==1)&&(eosens()))
      {
        lcd.print("Straight and left");
        decisions();
      }
    }
   //4-way/ T-int/ End of Maze
    else if((l==0)&&(c1==0)&&(c2==0)&&(c3==0)&&(r==0))
    {
      lcd.print("T/END/4");
      inch();
      if((l==1)&&(r==1)&&(eosens()))
      {
        lcd.print("4 way");
        decisions();
      }
      else if((l==1)&&(c1==1)&&(c2==1)&&(c3==1)&&(r==1))
      {
        lcd.print("T-int");
        decisions();
      }
      else if((l==0)&&(c1==0)&&(c2==0)&&(c3==0)&&(r==0))
     {
      lcd.print("End of Maze");
      right();
      delay(1000);
      final2();
     }
    }
  }
}
}
void decisions2()
{
  lcd.print("Entered decisions");
//  lcd.clear(); //clear the lcd
  if(rev[a]=='S')                    //if the element is S
  {
    forward();                            //forward
  //  delay(fdelay);                        //fdelay
  }
  if(rev[a]=='L')                  //if the element is L
  {
    right();                            //left
    delay(tdelay);                    //tdelay
  }
  if(rev[a]=='R')                  //if the element is R
  {
    left();                            //right
    delay(tdelay);                       //tdelay
  }
   a++;                                  //increment position in the array
}
//***********************************************************REVERSE SPA**************************************************************************************

void final2()
{
  lcd.clear();                          //clear the lcd
  lcd.print("ENTERED REVERSE SPA");
  delay(500);
  while(1)
  {
    lcd.clear();
   readsens();
   lcd.setCursor(1,1);
   //***************************************************************LINE FOLLOWER SENSORS************************************************************
   //Forward
  if((l==1)&&(c1==1)&&(c2==0)&&(c3==1)&&(r==1))
  {
    lcd.print("Forward");
    forward();
  }
  //Left
  else if(((l==1)&&(c1==0)&&(c2==1)&&(c3==1)&&(r==1))||( (l==1)&&(c1==0)&&(c2==0)&&(c3==1)&&(r==1)))
  {
    lcd.print("Left");
    smallleft();
  }
  //Right
  else if(( (l==1)&&(c1==1)&&(c2==1)&&(c3==0)&&(r==1))||( (l==1)&&(c1==1)&&(c2==0)&&(c3==0)&&(r==1)))
  {
    lcd.print("Right");
    smallright();
  }
  else if((l==1)&&(c1==1)&&(c2==1)&&(c3==1)&&(r==1))
  {
    while(1)
    {
      Stop();
      lcd.print("SUCCESS");
    }
  }
  
  //*******************************************MAZE 3 CATEGORIES*************************************************************************************
   else if(( (l==0)&&(c1==0))||((c3==0)&&(r==0)))
  {
    align();                  //align
    
    //RIGHT ONLY AND STRAIGHT AND RIGHT
    if(( (l==1)&&(c1==1)&&(c2==0)&&(c3==0)&&(r==0))||( (l==1)&&(c1==0)&&(c2==0)&&(c3==0)&&(r==0)))
    {
      lcd.print("Right or Straight and right?");
      inch();
      if((l==1)&&(c1==1)&&(c2==1)&&(c3==1)&&(r==1))
      {
        lcd.print("Right");
        right();              //right
        delay(tdelay);        //turn delay
      }
      else if((l==1)&&(r==1)&&(eosens()))
      {
        decisions2();
      }
    }
  //LEFT ONLY AND STRAIGHT LEFT
    else if(( (l==0)&&(c1==0)&&(c2==0)&&(c3==1)&&(r==1))||( (l==0)&&(c1==0)&&(c2==0)&&(c3==0)&&(r==1)))
    {
      lcd.print("Left or Straight and left");
      inch();
      if((l==1)&&(c1==1)&&(c2==1)&&(c3==1)&&(r==1))
      {
        lcd.print("Left");
        left();                 //left
        delay(tdelay);          //turn delay
      }
      else if((l==1)&&(r==1)&&(eosens()))
      {
        lcd.print("Straight and left");
        decisions2();
      }
    }
   //4-way/ T-int/ End of Maze
    else if((l==0)&&(c1==0)&&(c2==0)&&(c3==0)&&(r==0))
    {
      lcd.print("T/END/4");
      inch();
      if((l==1)&&(r==1)&&(eosens()))
      {
        lcd.print("4 way");
        decisions2();
      }
      else if((l==1)&&(c1==1)&&(c2==1)&&(c3==1)&&(r==1))
      {
        lcd.print("T-int");
        decisions2();
      }
     }
    }
  }
}


