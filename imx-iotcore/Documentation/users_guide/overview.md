---
title: "i.MX Windows 10 IoT User's Guide"
logo: [images/microsoftLogo.png, images/nxpLogo.png]
toc-own-page: true
titlepage-background: images/board.jpg
document-number: IMXWGU
...

Overview {#overview}
====

This chapter describes the process of building and installing the Windows 10 IoT OS BSP (Board Support Package) on the i.MX platform. It also covers special i.MX
features and how to use them.

The chapter also lists the steps to run the i.MX platform, including board DIP switch
settings, and instructions on configuring and using the U-Boot bootloader.

Features covered in this guide may be specific to particular boards or SOCs. For the capabilities
of a particular board or SOC, see the *i.MX Windows 10 IoT Release Notes*
(IMXWIN10RN).

Audience
----

This chapter is intended for software, hardware, and system engineers who are
planning to use the product, and for anyone who wants to know more about the
product.

Conventions
----

This chapter uses the following conventions:

- Courier New font: This font is used to identify commands, explicit command parameters, code examples,
expressions, data types, and directives.

Directories
---

BSP - Generated at build time. Contains Board Support Packages for the [IoT ADK AddonKit](https://github.com/ms-iot/iot-adk-addonkit).

build - Contains Board Packages, build scripts, and the VS2017 solution file.

driver - Contains driver sources.

documentation - Contains usage documentation.

hal - Contains hal extension sources.
