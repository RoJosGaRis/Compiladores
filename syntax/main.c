
int main()
{
  void* pParser = ParseAlloc (malloc);

  /* First input: 
      15 / 5
                                */
  Parse (pParser, PROGRAM, "program");
  Parse (pParser, ID, "smtn");
  Parse (pParser, SEMI_COLON, ";");
  Parse (pParser, 0, 0);

  ParseFree(pParser, free );

}
