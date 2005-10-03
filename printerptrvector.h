#ifndef printerptrvector_h
#define printerptrvector_h

class Printer;

/**
 * \class PrinterPtrVector
 * \brief This class implements a dynamic vector of Printer values
 */
class PrinterPtrVector {
public:
  /**
   * \brief This is the default PrinterPtrVector constructor
   */
  PrinterPtrVector() { size = 0; v = 0; };
  /**
   * \brief This is the PrinterPtrVector constructor for a specified size
   * \param sz is the size of the vector to be created
   * \note The elements of the vector will all be created, and set to zero
   */
  PrinterPtrVector(int sz);
  /**
   * \brief This is the PrinterPtrVector constructor for a specified size with an initial value
   * \param sz is the size of the vector to be created
   * \param initial is the initial value for all the entries of the vector
   */
  PrinterPtrVector(int sz, Printer* initial);
  /**
   * \brief This is the PrinterPtrVector destructor
   * \note This will free all the memory allocated to all the elements of the vector
   */
  ~PrinterPtrVector();
  /**
   * \brief This will add new entries to the vector
   * \param add is the number of new entries to the vector
   * \param value is the value that will be entered for the new entries
   */
  void resize(int add, Printer* value);
  /**
   * \brief This will add new empty entries to the vector
   * \param add is the number of new entries to the vector
   * \note The new elements of the vector will be created, and set to zero
   */
  void resize(int add);
  /**
   * \brief This will return the size of the vector
   * \return the size of the vector
   */
  int Size() const { return size; };
  /**
   * \brief This will return the value of an element of the vector
   * \param pos is the element of the vector to be returned
   * \return the value of the specified element
   */
  Printer*& operator [] (int pos) { return v[pos]; };
  /**
   * \brief This will return the value of an element of the vector
   * \param pos is the element of the vector to be returned
   * \return the value of the specified element
   */
  Printer* const& operator [] (int pos) const { return v[pos]; };
protected:
  /**
   * \brief This is the vector of Printer values
   */
  Printer** v;
  /**
   * \brief This is the size of the vector
   */
  int size;
};

#endif
