VAR  X, Y, Z, Q, R;

PROCEDURE  MULTIPLY;
  VAR  A, B;
  BEGIN  A := X;  B := Y;  Z := 0;
    WHILE B > 0 DO
    BEGIN  
      IF ODD B THEN Z := Z + A;
      A := 2*A ;  B := B/2 ;
    END
  END;
PROCEDURE  DIVIDE;
  VAR  W;
  BEGIN  R := X;  Q := 0;  W := Y;
    WHILE W <= R DO W := 2*W ;
    WHILE W > Y DO
    BEGIN  Q := 2*Q;  W := W/2;
      IF W <= R THEN
      BEGIN  R := R-W;  Q := Q+1 END
    END
  END;
PROCEDURE  GCD;
  VAR  F, G ;
  BEGIN  F := X;  G := Y;
    WHILE F # G DO
    BEGIN
      IF F < G THEN G := G-F;
      IF G < F THEN F := F-G;
    END;
    Z := F
  END;

BEGIN 
  ?X;  ?Y;  CALL MULTIPLY; !Z;
  ?X;  ?Y;  CALL DIVIDE; !Q; !R;
  ?X;  ?Y;  CALL GCD; !Z;
END.




