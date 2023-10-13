
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include "../osn/OSNReceiver.h"
#include "../osn/OSNSender.h"

namespace py = pybind11;

PYBIND11_MODULE(SSS, m)
{
    m.doc() = "Python bindings for OSNReceiver and OSNSender classes";

    // Wrap the OSNReceiver class
    py::class_<OSNReceiver>(m, "OSNReceiver")
        .def(py::init<>())
        .def("init", &OSNReceiver::init,
             py::arg("size"), py::arg("p"), py::arg("ot_type") = 1, py::arg("Sip") = "127.0.0.1:12345", py::arg("num_threads") = 1)
        .def("run_osn", &OSNReceiver::run_osn)
        .def("setTimer", &OSNReceiver::setTimer);

    // Wrap the OSNSender class
    py::class_<OSNSender>(m, "OSNSender")
        .def(py::init<>())
        .def("init", &OSNSender::init,
             py::arg("size"), py::arg("dest"), py::arg("p"), py::arg("ot_type") = 1, py::arg("Sip") = "127.0.0.1:12345", py::arg("num_threads") = 1)
        .def("run_osn", &OSNSender::run_osn)
        .def("setTimer", &OSNSender::setTimer);
}
