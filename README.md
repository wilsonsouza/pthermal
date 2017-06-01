Thermal Printer

This library to provider support for three printer models Epson, Daruma and Bematech.
This library use windows spool driver to generate ticket printed.
Is not necessary on generate none kind of configuration to use this library.
This library his make auto config.

example:

#include <thermalprinter.hpp>

void main()
{
  try
  {
    /* alloc shared memory safe, this pointer is auto deleted */
    LPThermalPrinter p = LPThermalPrinter(new ThermalPrinter());
    
    
  }
  catch(std::exception const & e)
  {
    std::cout << e.message() << std::endl;
  }
  finally
  {
  }
}
