/* -*- Mode: C++; indent-tabs-mode: nil -*- */
#ifndef SCHWA_EXCEPTION_H_
#define SCHWA_EXCEPTION_H_

#include <exception>
#include <iosfwd>
#include <string>

#include <schwa/_base.h>


namespace schwa {

  /**
   * Base class for libschwa exceptions. Useful when combined with \ref print_exception or for
   * catching all libschwa specific exceptions.
   **/
  class Exception : public std::exception {
  protected:
    std::string _msg;

  public:
    explicit Exception(const std::string &msg) : _msg(msg) { }
    Exception(const Exception &o) : std::exception(o), _msg(o._msg) { }
    virtual ~Exception(void) throw() { }

    inline const std::string &msg(void) const { return _msg; }
    virtual const char* what(void) const throw() override { return _msg.c_str(); }
  };


  /**
   * Exception subclass to indicate an error in the configuration framework. This includes both
   * the invalid setting up of the configuration framework itself and the invalid parsing of
   * configuration values.
   **/
  class ConfigException : public Exception {
  public:
    explicit ConfigException(const std::string &msg) : Exception(msg) { }
    ConfigException(const ConfigException &other) : Exception(other) { }
    virtual ~ConfigException(void) throw() { }
  };


  /**
   * Specific exception for describing I/O errors. They are documented with with an error message,
   * optional URI (file path), and optional line number within the file in question that the error
   * occurred on.
   **/
  class IOException : public Exception {
  protected:
    const std::string _uri;
    const int _linenum;

  public:
    explicit IOException(const std::string &msg, int linenum=-1);
    IOException(int errno_, const std::string &uri, int linenum=-1);
    IOException(const std::string &msg, const std::string &uri, int linenum=-1);
    IOException(const IOException &other);
    virtual ~IOException(void) throw() { }

    inline int linenum(void) const { return _linenum; }
    inline const std::string &uri(void) const { return _uri; }
  };


  /**
   * Specific exception to indicate that the wrong kind of value was provided.
   **/
  class ValueException : public Exception {
  public:
    explicit ValueException(const std::string &msg) : Exception(msg) { }
    ValueException(const ValueException &o) : Exception(o) { }
    virtual ~ValueException(void) throw() { }
  };


  /**
   * Exception used to quick escape to terminate ::main.
   **/
  class SystemExit : std::exception {
  protected:
    std::string _msg;
    int _exit_code;

  public:
    explicit SystemExit(int exit_code);
    SystemExit(const SystemExit &o) : std::exception(o), _exit_code(o._exit_code) { }
    virtual ~SystemExit(void) throw() { }

    inline int exit_code(void) const { return _exit_code; }
    inline const std::string &msg(void) const { return _msg; }
    virtual const char* what(void) const throw() override { return _msg.c_str(); }
  };




  /**
   * \ref Exception pretty printing helper.
   **/
  class print_exception {
  public:
    const std::string name;
    const Exception &e;

    explicit print_exception(const Exception &e);
    print_exception(const std::string &name, const Exception &e);

    std::ostream &dump(std::ostream &out) const;

  private:
    SCHWA_DISALLOW_COPY_AND_ASSIGN(print_exception);
  };

  std::ostream &operator <<(std::ostream &out, const print_exception &obj);

}

#endif  // SCHWA_EXCEPTION_H_
