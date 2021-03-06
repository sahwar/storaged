import unittest
import dbus
import sys
import subprocess
import os
import time

daemon_bin = None
test_devs = None

class StoragedTestCase(unittest.TestCase):
    iface_prefix = None
    path_prefix = None
    bus = None
    vdevs = None
    no_options = dbus.Dictionary(signature="sv")


    @classmethod
    def setUpClass(self):
        if daemon_bin == 'udisksd':
            self.iface_prefix = 'org.freedesktop.UDisks2'
            self.path_prefix = '/org/freedesktop/UDisks2'
        elif daemon_bin == 'storaged':
            self.iface_prefix = 'org.storaged.Storaged'
            self.path_prefix = '/org/storaged/Storaged'
        self.bus = dbus.SystemBus()
        self.vdevs = test_devs
        assert len(self.vdevs) > 3;


    @classmethod
    def tearDownClass(self):\
        pass


    @classmethod
    def get_object(self, iface_suffix, path_suffix):
        try:
            obj = self.bus.get_object(self.iface_prefix + iface_suffix, self.path_prefix + path_suffix)
        except:
            obj = None
        return obj


    @classmethod
    def get_property(self, obj, iface_suffix, prop):
        try:
            res = obj.Get(self.iface_prefix + iface_suffix, prop, dbus_interface=dbus.PROPERTIES_IFACE)
        except:
            res = None
        return res


    @classmethod
    def udev_settle(self):
        subprocess.call(['udevadm', 'settle'])

