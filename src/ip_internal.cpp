// ip_internal.cpp

// mkoctfile ip_internal.cpp Launcher.cpp LRBase.cpp

#include <octave/oct.h>
#include "Command.h"
#include "Launcher.h"

static Launcher &ipi_launcher() {
  static Launcher lnch("iplot");
  return lnch;
}

octave_value_list ipi_error(char const *msg) {
  fprintf(stderr, "%s\n", msg);
  Matrix m(0, 0);
  octave_value_list res;
  for (int k=0; k<5; k++)
    res(k) = m;
  return res;
}

octave_value_list ipi_send(octave_value_list const &args) {
  int nargin = args.length();
  if (nargin<2)
    return ipi_error("ipi_send: Must have at least command and handle");
  else if (nargin>2+Launcher::MaxArgs)
    return ipi_error("ipi_send: Too many arguments");

  charMatrix cmd_ch = args(0).char_matrix_value();
  if (error_state || cmd_ch.numel()==0)
    return ipi_error("ipi_send: Command must be a string");
  std::string cmd_str = cmd_ch.row_as_string(0);

  Command::Command cmd = Command::BadCommand;
  if (cmd_str=="figure")
    cmd = Command::Figure;
  else if (cmd_str=="close")
    cmd = Command::Close;
  else if (cmd_str=="axes")
    cmd = Command::Axes;
  else if (cmd_str=="line")
    cmd = Command::Line;
  else if (cmd_str=="points")
    cmd = Command::Points;
  else if (cmd_str=="patch")
    cmd = Command::Patch;
  else if (cmd_str=="subplot")
    cmd = Command::Subplot;
  else if (cmd_str=="text")
    cmd = Command::Text;
  else if (cmd_str=="button")
    cmd = Command::Button;
  else if (cmd_str=="set")
    cmd = Command::Set;
  else if (cmd_str=="get")
    cmd = Command::Get;
  else if (cmd_str=="properties")
    cmd = Command::Properties;
  else if (cmd_str=="find")
    cmd = Command::Find;
  else if (cmd_str=="delete")
    cmd = Command::Delete;
  else if (cmd_str=="gcf")
    cmd = Command::GCF;
  else if (cmd_str=="gca")
    cmd = Command::GCA;
  else if (cmd_str=="gcbo")
    cmd = Command::GCBO;
  else if (cmd_str=="gcbf")
    cmd = Command::GCBF;
  else
    return ipi_error("ipi_send: Bad command");

  uint64NDArray hdl(args(1).uint64_array_value());
  if (hdl.numel()!=1)
    return ipi_error("ipi_send: Handle must be a scalar");

  Launcher::Header hdr;
  Launcher::prepareHeader(&hdr);
  hdr.cmd = cmd;
  hdr.handle = hdl(0);
  hdr.nargs = nargin - 2;
  NDArray mat[Launcher::MaxArgs];
  charMatrix chr[Launcher::MaxArgs];
  void const *dat[Launcher::MaxArgs];
  for (int k=0; k<Launcher::MaxArgs; k++)
    dat[k] = NULL;
  for (int k=0; k<hdr.nargs; k++) {
    if (args(2+k).is_sq_string() || args(2+k).is_dq_string()) {
      chr[k] = args(2+k).char_matrix_value();
      hdr.argtype[k] = Launcher::DT_Char;
      hdr.arglen[k] = chr[k].numel();
      dat[k] = chr[k].fortran_vec();
    } else {
      mat[k] = args(2+k).array_value();
      hdr.argtype[k] = Launcher::DT_Double;
      hdr.arglen[k] = mat[k].numel()*sizeof(double);
      dat[k] = mat[k].fortran_vec();
    }
    if (error_state)
      return ipi_error("ipi_send: Bad arguments");
  }

  if (ipi_launcher().send(&hdr, dat[0], dat[1])) {
    // Success
    uint64NDArray seqid(dim_vector(1, 1), 0);
    seqid(0) = hdr.seqid;
    return octave_value(seqid);
  } else {
    return ipi_error("ipi_send: Failed to send");
  }
}

octave_value_list ipi_receive_finite(double timeout_ms) {
  uint64NDArray seqid(dim_vector(1, 1), 0);
  if (!ipi_launcher().receive(int(timeout_ms))) {
    seqid(0) = 0;
    ipi_error("ipi_receive: Timeout");
    return octave_value(seqid);
  }
  Launcher::Header const &hdr = ipi_launcher().receivedHeader();
  seqid(0) = hdr.seqid;
  charMatrix cmd_ch;
  switch ((Command::Command)hdr.cmd) {
  case Command::OK: cmd_ch = "ok"; break;
  case Command::Handle: cmd_ch = "handle"; break;
  case Command::Callback: cmd_ch = "callback"; break;
  case Command::Value: cmd_ch = "value"; break;
  case Command::BadCommand: cmd_ch = "badcommand"; break;
  case Command::BadHandle: cmd_ch = "badhandle"; break;
  case Command::BadKey: cmd_ch = "badkey"; break;
  case Command::BadValue: cmd_ch = "badvalue"; break;
  case Command::BadSyntax: cmd_ch = "badsyntax"; break;
  default: cmd_ch = "unknown";
    fprintf(stderr, "Unknown response from iplot: %i\n", hdr.cmd);
    break;
  }
  uint64NDArray hdl(1);
  hdl(0) = hdr.handle;

  octave_value arg[Launcher::MaxArgs];
  for (int k=0; k<Launcher::MaxArgs; k++)
    arg[k] = NDArray();

  for (int k=0; k<hdr.nargs; k++) {
    switch ((Launcher::DataType)hdr.argtype[k]) {
    case Launcher::DT_Char: {
      charMatrix m(1, hdr.arglen[k]);
      memcpy(m.fortran_vec(), ipi_launcher().receivedArg(k), hdr.arglen[k]);
      arg[k] = m;
    } break;
    case Launcher::DT_Int32: {
      int32NDArray m(hdr.arglen[k]/sizeof(int32_t));
      memcpy(m.fortran_vec(), ipi_launcher().receivedArg(k), hdr.arglen[k]);
      arg[k] = m;
    } break;
    case Launcher::DT_UInt64: {
      uint64NDArray m(hdr.arglen[k]/sizeof(uint64_t));
      memcpy(m.fortran_vec(), ipi_launcher().receivedArg(k), hdr.arglen[k]);
      arg[k] = m;
    } break;
    case Launcher::DT_Float: {
      FloatMatrix m(hdr.arglen[k]/sizeof(float), 1);
      memcpy(m.fortran_vec(), ipi_launcher().receivedArg(k), hdr.arglen[k]);
      arg[k] = m;
    } break;
    case Launcher::DT_Double: {
      Matrix m(hdr.arglen[k]/sizeof(double), 1);
      memcpy(m.fortran_vec(), ipi_launcher().receivedArg(k), hdr.arglen[k]);
      arg[k] = m;
    } break;
    default:
      return ipi_error("ipi_recv: Unknown data type");
    }
  }
  octave_value_list res;
  res(0) = seqid;
  res(1) = cmd_ch;
  res(2) = hdl;
  for (int k=0; k<Launcher::MaxArgs; k++)
    res(3+k) = arg[k];
  return res;  
}

octave_value_list ipi_receive_indefinitely() {
  return ipi_receive_finite(1e9); // a million seconds
}

DEFUN_DLD(ip_internal, args, /*nargout*/,	
  "IP_INTERNAL - Single entry point for octave to call IPlot\n"
  "   id = IP_INTERNAL(cmd, h, arg1, arg2) sends a command; ARG1 and ARG2\n"
  "   are optional.\n"
  "   [id, cmd, h, arg1, arg2] = IP_INTERNAL waits indefinitely to\n"
  "   receive a command.\n"
  "   [id, cmd, h, arg1, arg2] = IP_INTERNAL(timeout_ms) waits for the given\n"
  "   number of milliseconds to receive a response. If none is received,\n"
  "   ID = 0 on return.\n") {
  int nargin = args.length();
  if (nargin==0) {
    return ipi_receive_indefinitely();
  } else if (nargin==1) {
    NDArray timeout(args(0).array_value());
    if (error_state || timeout.numel()==0)
      return ipi_error("Illegal timeout");
    else
      return ipi_receive_finite(timeout(0));
  } else {
    return ipi_send(args);
  }
}
