/*
  Copyright 2013  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/
/*******************************************************************************
 Copyright (c) 2014, Infineon Technologies AG                                 **
 All rights reserved.                                                         **
                                                                              **
 Redistribution and use in source and binary forms, with or without           **
 modification,are permitted provided that the following conditions are met:   **
                                                                              **
 *Redistributions of source code must retain the above copyright notice,      **
 this list of conditions and the following disclaimer.                        **
 *Redistributions in binary form must reproduce the above copyright notice,   **
 this list of conditions and the following disclaimer in the documentation    **
 and/or other materials provided with the distribution.                       **
 *Neither the name of the copyright holders nor the names of its contributors **
 may be used to endorse or promote products derived from this software without**
 specific prior written permission.                                           **
                                                                              **
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"  **
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE    **
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE   **
 ARE  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE   **
 LIABLE  FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR         **
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF         **
 SUBSTITUTE GOODS OR  SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS    **
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN      **
 CONTRACT, STRICT LIABILITY,OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)       **
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE   **
 POSSIBILITY OF SUCH DAMAGE.                                                  **
                                                                              **
 To improve the quality of the software, users are encouraged to share        **
 modifications, enhancements or bug fixes with Infineon Technologies AG       **
 dave@infineon.com).                                                          **
                                                                              **
********************************************************************************
**                                                                            **
**                                                                            **
** PLATFORM : Infineon XMC4000 Series                                         **
**                                                                            **
** COMPILER : Compiler Independent                                            **
**                                                                            **
** AUTHOR : App Developer                                                     **
**                                                                            **
** MAY BE CHANGED BY USER [yes/no]: No                                        **
**                                                                            **
** MODIFICATION DATE : Feb 21, 2014     		                              **
**                                                                            **
*******************************************************************************/

/** \file
 *  \brief Common standard USB Descriptor definitions for all architectures.
 *  \copydetails Group_StdDescriptors
 *
 */

/** \ingroup Group_USB
 *  \defgroup Group_StdDescriptors USB Descriptors
 *  \brief Standard USB Descriptor definitions.
 *
 *  Standard USB device descriptor defines and retrieval routines, for USB
 *  devices. This module contains structures and macros for the easy creation
 *  of standard USB descriptors in USB device projects.
 *
 *  @{
 */

#ifndef __USBDESCRIPTORS_H__
#define __USBDESCRIPTORS_H__

	/* Includes: */
		#include "../Common/Common.h"
		#include "USBMode.h"
		#include "Events.h"

	/* Enable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			extern "C" {
		#endif


	/* Public Interface - May be used in end-application: */
		/* Macros: */
			/** Indicates that a given descriptor does not exist in the device.
			 * This can be used inside descriptors for string descriptor
			 * indexes, or may be use as a return value for GetDescriptor when
			 * the specified descriptor does not exist.
			 */
			#define NO_DESCRIPTOR                     0

			/** Macro to calculate the power value for the configuration
			 * descriptor, from a given number of milliamperes.
			 *
			 *  \param[in] mA  Maximum number of milliamps the device consumes
			 *  when the given configuration is selected.
			 */
			#define USB_CONFIG_POWER_MA(mA)           ((mA) >> 1)

			/** Macro to calculate the Unicode length of a string with a given
			 * number of Unicode characters. Should be used in string
			 * descriptor's headers for giving the string descriptor's byte
			 * length.
			 *
			 *  \param[in] UnicodeChars  Number of Unicode characters in the
			 *  string text.
			 */
			#define USB_STRING_LEN(UnicodeChars)      \
			(sizeof(USB_Descriptor_Header_t) + ((UnicodeChars) << 1))

			/** Macro to encode a given four digit floating point version number
			 *  (e.g. 01.23) into Binary Coded  Decimal format for descriptor
			 *  fields requiring BCD encoding, such as the USB version number in
			 *   the standard device descriptor.
			 *
			 *  \note This value is automatically converted into Little Endian,
			 *  suitable for direct use inside device descriptors on all
			 *  architectures without endianness conversion macros.
			 *
			 *  \param[in]  x  Version number to encode as a 16-bit little-
			 *  endian number, as a floating point number.
			 */
			#define VERSION_BCD(x)                    \
			CPU_TO_LE16((VERSION_TENS(x) << 12)  | (VERSION_ONES(x) << 8) | \
				  (VERSION_TENTHS(x) << 4) | (VERSION_HUNDREDTHS(x) << 0) )

			/** String language ID for the English language. Should be used in
			 * \ref USB_Descriptor_String_t descriptors to indicate that the
			 * English language is supported by the device in its string
			 * descriptors.
			 */
			#define LANGUAGE_ID_ENG                   0x0409

			/** \name USB Configuration Descriptor Attribute Masks */
			//@{
			/** Mask for the reserved bit in the Configuration Descriptor's
			 * \c ConfigAttributes field, which must be set on all
			 *  devices for historical purposes.
			 */
			#define USB_CONFIG_ATTR_RESERVED          0x80

			/** Can be masked with other configuration descriptor attributes for
			 *  a \ref USB_Descriptor_Configuration_Header_t  descriptor's
			 *  \c ConfigAttributes value to indicate that the specified
			 *  configuration can draw its power from the device's own power
			 *  source.
			 */
			#define USB_CONFIG_ATTR_SELFPOWERED       0x40

			/** Can be masked with other configuration descriptor attributes
			 * for a \ref USB_Descriptor_Configuration_Header_t
			 *  descriptor's \c ConfigAttributes value to indicate that the
			 *  specified configuration supports the remote wakeup feature of
			 *  the USB standard, allowing a suspended USB device to wake up the
			 *   host upon  request.
			 */
			#define USB_CONFIG_ATTR_REMOTEWAKEUP      0x20
			//@}

			/** \name Endpoint Descriptor Attribute Masks */
			//@{
			/** Can be masked with other endpoint descriptor attributes for a
			 * \ref USB_Descriptor_Endpoint_t descriptor's \c Attributes value
			 * to indicate that the specified endpoint is not synchronized.
			 *
			 *  \see The USB specification for more details on the possible
			 *  Endpoint attributes.
			 */
			#define ENDPOINT_ATTR_NO_SYNC             (0 << 2)

			/** Can be masked with other endpoint descriptor attributes for a
			 * \ref USB_Descriptor_Endpoint_t descriptor's \c Attributes value
			 * to indicate that the specified endpoint is asynchronous.
			 *
			 *  \see The USB specification for more details on the possible
			 *  Endpoint attributes.
			 */
			#define ENDPOINT_ATTR_ASYNC               (1 << 2)

			/** Can be masked with other endpoint descriptor attributes for a
			 * \ref USB_Descriptor_Endpoint_t descriptor's \c Attributes value
			 * to indicate that the specified endpoint is adaptive.
			 *
			 *  \see The USB specification for more details on the possible
			 *  Endpoint attributes.
			 */
			#define ENDPOINT_ATTR_ADAPTIVE            (2 << 2)

			/** Can be masked with other endpoint descriptor attributes for a
			 * \ref USB_Descriptor_Endpoint_t descriptor's
			 *  \c Attributes value to indicate that the specified endpoint is
			 *  synchronized.
			 *
			 *  \see The USB specification for more details on the possible
			 *  Endpoint attributes.
			 */
			#define ENDPOINT_ATTR_SYNC                (3 << 2)
			//@}

			/** \name Endpoint Descriptor Usage Masks */
			//@{
			/** Can be masked with other endpoint descriptor attributes for a
			 * \ref USB_Descriptor_Endpoint_t descriptor's
			 *  \c Attributes value to indicate that the specified endpoint is
			 *  used for data transfers.
			 *
			 *  \see The USB specification for more details on the possible
			 *  Endpoint usage attributes.
			 */
			#define ENDPOINT_USAGE_DATA               (0 << 4)

			/** Can be masked with other endpoint descriptor attributes for a
			 * \ref USB_Descriptor_Endpoint_t descriptor's
			 *  \c Attributes value to indicate that the specified endpoint is
			 *  used for feedback.
			 *
			 *  \see The USB specification for more details on the possible
			 *  Endpoint usage attributes.
			 */
			#define ENDPOINT_USAGE_FEEDBACK           (1 << 4)

			/** Can be masked with other endpoint descriptor attributes for a
			 * \ref USB_Descriptor_Endpoint_t descriptor's \c Attributes value
			 * to indicate that the specified endpoint is used for implicit
			 * feedback.
			 *
			 *  \see The USB specification for more details on the possible
			 *  Endpoint usage attributes.
			 */
			#define ENDPOINT_USAGE_IMPLICIT_FEEDBACK  (2 << 4)
			//@}

		/* Enums: */
			/** Enum for the possible standard descriptor types, as given in
			 * each descriptor's header. */
			enum USB_DescriptorTypes_t
			{
				/**< Indicates that the descriptor is a device descriptor. */
				DTYPE_Device                    = 0x01,
				/**< Indicates that the descriptor is a configuration
				 * descriptor. */
				DTYPE_Configuration             = 0x02,
				/**< Indicates that the descriptor is a string descriptor. */
				DTYPE_String                    = 0x03,
				/**< Indicates that the descriptor is an interface descriptor.*/
				DTYPE_Interface                 = 0x04,
				/**< Indicates that the descriptor is an endpoint descriptor. */
				DTYPE_Endpoint                  = 0x05,
				/**< Indicates that the descriptor is a device qualifier
				 * descriptor. */
				DTYPE_DeviceQualifier           = 0x06,
				/**< Indicates that the descriptor is of other type. */
				DTYPE_Other                     = 0x07,
				/**< Indicates that the descriptor is an interface power
				 * descriptor. */
				DTYPE_InterfacePower            = 0x08,
				/**< Indicates that the descriptor is an interface association
				 * descriptor. */
				DTYPE_InterfaceAssociation      = 0x0B,
				/**< Indicates that the descriptor is a class specific
				 * interface descriptor. */
				DTYPE_CSInterface               = 0x24,
				/**< Indicates that the descriptor is a class specific endpoint
				 * descriptor. */
				DTYPE_CSEndpoint                = 0x25,
			};

			/** Enum for possible Class, Subclass and Protocol values of device
			 * and interface descriptors. */
			enum USB_Descriptor_ClassSubclassProtocol_t
			{
				/**< Descriptor Class value indicating that the device does not
				 * belong to a particular class at the device level. */
				USB_CSCP_NoDeviceClass          = 0x00,
				/**< Descriptor Subclass value indicating that the device does
				 * not belong to a particular subclass at the device level. */
				USB_CSCP_NoDeviceSubclass       = 0x00,
				/**< Descriptor Protocol value indicating that the device does
				 * not belong to a particular protocol at the device level. */
				USB_CSCP_NoDeviceProtocol       = 0x00,
				/**< Descriptor Class value indicating that the device/interface
				 *  belongs to a vendor specific class. */
				USB_CSCP_VendorSpecificClass    = 0xFF,
				/**< Descriptor Subclass value indicating that the device/
				 * interface belongs to a vendor specific subclass.*/
				USB_CSCP_VendorSpecificSubclass = 0xFF,
				/**< Descriptor Protocol value indicating that the device/
				 * interface belongs to a vendor specific protocol. */
				USB_CSCP_VendorSpecificProtocol = 0xFF,
				/**< Descriptor Class value indicating that the device belongs
				 * to the Interface Association Descriptor class. */
				USB_CSCP_IADDeviceClass         = 0xEF,
				/**< Descriptor Subclass value indicating that the device
				 * belongs to the Interface Association Descriptor subclass.*/
				USB_CSCP_IADDeviceSubclass      = 0x02,
				/**< Descriptor Protocol value indicating that the device
				 * belongs to the Interface Association Descriptor protocol. */
				USB_CSCP_IADDeviceProtocol      = 0x01,
			};

		/* Type Defines: */
			/** \brief Standard USB Descriptor Header (LUFA naming conventions).
			 *
             *  Type define for all descriptors' standard header, indicating the
             *   descriptor's length and type. This structure
			 *  uses LUFA-specific element names to make each element's purpose
			 *  clearer.
			 *
			 *  \see \ref USB_StdDescriptor_Header_t for the version of this
			 *  type with standard element names.
			 *
			 *  \note Regardless of CPU architecture, these values should be
			 *  stored as little endian.
			 */
			IAR_ATTR_PACKED
			struct TASKING_ATTR_PACKED USB_Descriptor_Header
			{
				/**< Size of the descriptor, in bytes. */
				uint8_t Size;

				/**< Type of the descriptor, either a value in
				 * \ref USB_DescriptorTypes_t or a value given by the specific
				 * class. */
				uint8_t Type;
			}ATTR_PACKED;

			typedef struct USB_Descriptor_Header USB_Descriptor_Header_t;

			/** \brief Standard USB Descriptor Header (USB-IF naming
			 * conventions).
			 *
			 *  Type define for all descriptors' standard header, indicating the
			 *   descriptor's length and type. This structure
			 *  uses the relevant standard's given element names to ensure
			 *  compatibility with the standard.
			 *
			 *  \see \ref USB_Descriptor_Header_t for the version of this type
			 *  with non-standard LUFA specific element names.
			 *
			 *  \note Regardless of CPU architecture, these values should be
			 *  stored as little endian.
			 */
			IAR_ATTR_PACKED
			struct TASKING_ATTR_PACKED USB_StdDescriptor_Header
			{
				/**< Size of the descriptor, in bytes. */
				uint8_t bLength;

				/**< Type of the descriptor, either a value in
				 * \ref USB_DescriptorTypes_t or a value given by the specific
				 * class.*/
				uint8_t bDescriptorType;
			}ATTR_PACKED;
			typedef struct USB_StdDescriptor_Header USB_StdDescriptor_Header_t;

			/** \brief Standard USB Device Descriptor (LUFA naming conventions).
			 *
			 *  Type define for a standard Device Descriptor. This structure
			 *  uses LUFA-specific element names to make each element's purpose
			 *  clearer.
			 *
			 *  \see \ref USB_StdDescriptor_Device_t for the version of this
			 *  type with standard element names.
			 *
			 *  \note Regardless of CPU architecture, these values should be
			 *  stored as little endian.
			 */
			IAR_ATTR_PACKED struct TASKING_ATTR_PACKED USB_Descriptor_Device
			{
				/**< Descriptor header, including type and size. */
				USB_Descriptor_Header_t Header;

				/**< BCD of the supported USB specification. */
				uint16_t USBSpecification;

				/**< USB device class. */
				uint8_t  Class;

				/**< USB device subclass. */
				uint8_t  SubClass;

				/**< USB device protocol. */
				uint8_t  Protocol;

				/**< Size of the control (address 0) endpoint's bank in
				 * bytes. */
				uint8_t  Endpoint0Size;

				/**< Vendor ID for the USB product. */
				uint16_t VendorID;

				/**< Unique product ID for the USB product. */
				uint16_t ProductID;

				/**< Product release (version) number. */
				uint16_t ReleaseNumber;

				/**< String index for the manufacturer's name. The host will
				 * request this string via a separate control request for the
				 * string descriptor.If no string supplied, use
				 * \ref NO_DESCRIPTOR. */
				uint8_t  ManufacturerStrIndex;

				/**< String index for the product name/details.*/
				uint8_t  ProductStrIndex;

				/**< String index for the product's globally unique hexadecimal
				 * serial number, in uppercase Unicode ASCII. \note On some
				 * microcontroller models, there is an embedded serial number in
				 *  the chip which can be used for the device serial number. */
				uint8_t  SerialNumStrIndex;

				/**< Total number of configurations supported by the device. */
				uint8_t  NumberOfConfigurations;
			}ATTR_PACKED;
			typedef struct USB_Descriptor_Device USB_Descriptor_Device_t;

			/** \brief Standard USB Device Descriptor
			 * (USB-IF naming conventions).
			 *
			 *  Type define for a standard Device Descriptor. This structure
			 *  uses the relevant standard's given element names to ensure
			 *  compatibility with the standard.
			 *
			 *  \see \ref USB_Descriptor_Device_t for the version of this type
			 *  with non-standard LUFA specific element names.
			 *
			 *  \note Regardless of CPU architecture, these values should be
			 *  stored as little endian.
			 */
			IAR_ATTR_PACKED struct TASKING_ATTR_PACKED USB_StdDescriptor_Device
			{
				/**< Size of the descriptor, in bytes. */
				uint8_t  bLength;

				/**< Type of the descriptor, either a value in
				 * \ref USB_DescriptorTypes_t or a value given by the specific
				 * class.*/
				uint8_t  bDescriptorType;

				/**< BCD of the supported USB specification. */
				uint16_t bcdUSB;

				/**< USB device class. */
				uint8_t  bDeviceClass;

				/**< USB device subclass. */
				uint8_t  bDeviceSubClass;

				/**< USB device protocol. */
				uint8_t  bDeviceProtocol;

				/**< Size of the control (address 0) endpoint's bank in
				 * bytes. */
				uint8_t  bMaxPacketSize0;

				/**< Vendor ID for the USB product. */
				uint16_t idVendor;

				/**< Unique product ID for the USB product. */
				uint16_t idProduct;

				/**< Product release (version) number. */
				uint16_t bcdDevice;

				/**< String index for the manufacturer's name. The host will
				 * request this string via a separate control request for the
				 * string descriptor.\note If no string supplied, use
				 * \ref NO_DESCRIPTOR. */
				uint8_t  iManufacturer;

				/**< String index for the product name/details. */
				uint8_t  iProduct;

				/**< String index for the product's globally unique hexadecimal
				 * serial number, in uppercase Unicode ASCII.*/
				uint8_t iSerialNumber;

				/**< Total number of configurations supported by the device.*/
				uint8_t  bNumConfigurations;
			}ATTR_PACKED;
			typedef struct USB_StdDescriptor_Device USB_StdDescriptor_Device_t;

			/** \brief Standard USB Device Qualifier Descriptor (LUFA naming
			 * conventions).
			 *
			 *  Type define for a standard Device Qualifier Descriptor. This
			 *  structure uses LUFA-specific element names
			 *  to make each element's purpose clearer.
			 *
			 *  \see \ref USB_StdDescriptor_DeviceQualifier_t for the version of
			 *   this type with standard element names.
			 */
			IAR_ATTR_PACKED
			struct TASKING_ATTR_PACKED USB_Descriptor_DeviceQualifier
			{
				/**< Descriptor header, including type and size. */
				USB_Descriptor_Header_t Header;

				/**< BCD of the supported USB specification. */
				uint16_t USBSpecification;

				/**< USB device class. */
				uint8_t  Class;

				/**< USB device subclass. */
				uint8_t  SubClass;

				/**< USB device protocol. */
				uint8_t  Protocol;

				/**< Size of the control (address 0) endpoint's bank in
				 * bytes. */
				uint8_t  Endpoint0Size;

				/**< Total number of configurations supported by the device. */
				uint8_t  NumberOfConfigurations;

				/**< Reserved for future use, must be 0. */
				uint8_t  Reserved;
			}ATTR_PACKED;
			typedef struct USB_Descriptor_DeviceQualifier
							USB_Descriptor_DeviceQualifier_t;

			/** \brief Standard USB Device Qualifier Descriptor (USB-IF naming
			 * conventions).
			 *
			 *  Type define for a standard Device Qualifier Descriptor. This
			 *  structure uses the relevant standard's given element names
			 *  to ensure compatibility with the standard.
			 *
			 *  \see \ref USB_Descriptor_DeviceQualifier_t for the version of
			 *  this type with non-standard LUFA specific element names.
			 */
			IAR_ATTR_PACKED
			struct TASKING_ATTR_PACKED USB_StdDescriptor_DeviceQualifier
			{
				/**< Size of the descriptor, in bytes. */
				uint8_t  bLength;

				/**< Type of the descriptor, either a value in
				 * \ref USB_DescriptorTypes_t or a value given by the specific
				 * class. */
				uint8_t  bDescriptorType;

				/**< BCD of the supported USB specification. */
				uint16_t bcdUSB;

				/**< USB device class. */
				uint8_t  bDeviceClass;

				/**< USB device subclass. */
				uint8_t  bDeviceSubClass;

				/**< USB device protocol. */
				uint8_t  bDeviceProtocol;

				/**< Size of the control (address 0) endpoint's bank in
				 * bytes. */
				uint8_t  bMaxPacketSize0;

				/**< Total number of configurations supported by the device.*/
				uint8_t  bNumConfigurations;

				/**< Reserved for future use, must be 0. */
				uint8_t  bReserved;
			}ATTR_PACKED;
			typedef struct USB_StdDescriptor_DeviceQualifier
							USB_StdDescriptor_DeviceQualifier_t;

			/** \brief Standard USB Configuration Descriptor (LUFA naming
			 * conventions).
			 *
			 *  Type define for a standard Configuration Descriptor header. This
			 *   structure uses LUFA-specific element names to make each
			 *   element's purpose clearer. \see
			 *   \ref USB_StdDescriptor_Configuration_Header_t for the version
			 *   of this type with standard element names.
			 *
			 *  \note Regardless of CPU architecture, these values should be
			 *  stored as little endian.
			 */
			IAR_ATTR_PACKED
			struct TASKING_ATTR_PACKED USB_Descriptor_Configuration_Header
			{
				/**< Descriptor header, including type and size. */
				USB_Descriptor_Header_t Header;

				/**< Size of the configuration descriptor header, and all sub
				 * descriptors inside the configuration. */
				uint16_t TotalConfigurationSize;

				/**< Total number of interfaces in the configuration. */
				uint8_t  TotalInterfaces;

				/**< Configuration index of the current configuration. */
				uint8_t  ConfigurationNumber;

				/**< Index of a string descriptor describing the
				 * configuration. */
				uint8_t  ConfigurationStrIndex;

				/**< Configuration attributes, comprised of a mask of
				 * \c USB_CONFIG_ATTR_* masks.On all devices, this should
				 * include USB_CONFIG_ATTR_RESERVED at a minimum. */
				uint8_t  ConfigAttributes;

				/**< Maximum power consumption of the device while in the
				 * current configuration, calculated by the
				 * \ref USB_CONFIG_POWER_MA() macro.*/
				uint8_t  MaxPowerConsumption;
			}ATTR_PACKED;
			typedef struct USB_Descriptor_Configuration_Header
						USB_Descriptor_Configuration_Header_t;

			/** \brief Standard USB Configuration Descriptor (USB-IF naming
			 * conventions).
			 *
			 *  Type define for a standard Configuration Descriptor header.
			 *  This structure uses the relevant standard's given element names
			 *  to ensure compatibility with the standard.
			 *
			 *  \see \ref USB_Descriptor_Device_t for the version of this type
			 *  with non-standard LUFA specific element names.
			 *
			 *  \note Regardless of CPU architecture, these values should be
			 *  stored as little endian.
			 */
			IAR_ATTR_PACKED
			struct TASKING_ATTR_PACKED USB_StdDescriptor_Configuration_Header
			{
				/**< Size of the descriptor, in bytes. */
				uint8_t  bLength;

				/**< Type of the descriptor, either a value in
				 * \ref USB_DescriptorTypes_t or a value  given by the specific
				 * class.*/
				uint8_t  bDescriptorType;

				/**< Size of the configuration descriptor header, and all sub
				 * descriptors inside the configuration.*/
				uint16_t wTotalLength;

				/**< Total number of interfaces in the configuration. */
				uint8_t  bNumInterfaces;

				/**< Configuration index of the current configuration. */
				uint8_t  bConfigurationValue;

				/**< Index of a string descriptor describing the
				 * configuration. */
				uint8_t  iConfiguration;

				/**< Configuration attributes, comprised of a mask of
				 * \c USB_CONFIG_ATTR_* masks. On all devices, this should
				 * include USB_CONFIG_ATTR_RESERVED at a minimum.
								                        */
				uint8_t  bmAttributes;

				/**< Maximum power consumption of the device while in the
				 * current configuration, calculated by the
				 * \ref USB_CONFIG_POWER_MA() macro.*/
				uint8_t  bMaxPower;
			}ATTR_PACKED;
			typedef struct USB_StdDescriptor_Configuration_Header
						USB_StdDescriptor_Configuration_Header_t;

			/** \brief Standard USB Interface Descriptor (LUFA naming
			 * conventions).
			 *
			 *  Type define for a standard Interface Descriptor. This structure
			 *  uses LUFA-specific element names to make each element's purpose
			 *  clearer.
			 *
			 *  \see \ref USB_StdDescriptor_Interface_t for the version of this
			 *  type with standard element names.
			 *
			 *  \note Regardless of CPU architecture, these values should be
			 *  stored as little endian.
			 */
			IAR_ATTR_PACKED struct TASKING_ATTR_PACKED USB_Descriptor_Interface
			{
				/**< Descriptor header, including type and size. */
				USB_Descriptor_Header_t Header;

				/**< Index of the interface in the current configuration. */
				uint8_t InterfaceNumber;

				/**< Alternate setting for the interface number. The same
				 * interface number can have multiple alternate settings with
				 * different endpoint configurations, which can be selected by
				 * the host.*/
				uint8_t AlternateSetting;

				/**< Total number of endpoints in the interface. */
				uint8_t TotalEndpoints;

				/**< Interface class ID. */
				uint8_t Class;

				/**< Interface subclass ID. */
				uint8_t SubClass;

				/**< Interface protocol ID. */
				uint8_t Protocol;

				/**< Index of the string descriptor describing the interface.*/
				uint8_t InterfaceStrIndex;
			}ATTR_PACKED;
			typedef struct USB_Descriptor_Interface USB_Descriptor_Interface_t;

			/** \brief Standard USB Interface Descriptor (USB-IF naming
			 * conventions).
			 *
			 *  Type define for a standard Interface Descriptor. This structure
			 *  uses the relevant standard's given element names to ensure
			 *  compatibility with the standard.
			 *
			 *  \see \ref USB_Descriptor_Interface_t for the version of this
			 *  type with non-standard LUFA specific element names.
			 *
			 *  \note Regardless of CPU architecture, these values should be
			 *  stored as little endian.
			 */
			IAR_ATTR_PACKED
			struct TASKING_ATTR_PACKED USB_StdDescriptor_Interface
			{
				/**< Size of the descriptor, in bytes. */
				uint8_t bLength;

				/**< Type of the descriptor, either a value in
				 * \ref USB_DescriptorTypes_t or a value given by the specific
				 * class.*/
				uint8_t bDescriptorType;

				/**< Index of the interface in the current configuration. */
				uint8_t bInterfaceNumber;

				/**< Alternate setting for the interface number. The same
				 * interface number can have multiple alternate settings with
				 * different endpoint configurations, which can be selected by
				 * the host.*/
				uint8_t bAlternateSetting;

				/**< Total number of endpoints in the interface. */
				uint8_t bNumEndpoints;

				/**< Interface class ID. */
				uint8_t bInterfaceClass;

				/**< Interface subclass ID. */
				uint8_t bInterfaceSubClass;

				/**< Interface protocol ID. */
				uint8_t bInterfaceProtocol;

				/**< Index of the string descriptor describing the interface.*/
				uint8_t iInterface;
			}ATTR_PACKED;
			typedef struct USB_StdDescriptor_Interface
								USB_StdDescriptor_Interface_t;

			/** \brief Standard USB Interface Association Descriptor (LUFA
			 * naming conventions).
			 *
			 *  Type define for a standard Interface Association Descriptor.
			 *  This structure uses LUFA-specific element names make each
			 *  element's purpose clearer.
			 *
			 *  This descriptor has been added as a supplement to the USB2.0
			 *  standard, in the ECN located at <a>http://www.usb.org/developers
			 *  /docs/InterfaceAssociationDescriptor_ecn.pdf</a>. It allows
			 *  composite devices with multiple interfaces related to the same
			 *  function to have the multiple interfaces bound together at the
			 *  point of enumeration, loading one generic driver for all the
			 *  interfaces in the single function. Read the ECN for more
			 *  information.
			 *
			 *  \see \ref USB_StdDescriptor_Interface_Association_t for the
			 *  version of this type with standard element names.
			 *
			 *  \note Regardless of CPU architecture, these values should be
			 *  stored as little endian.
			 */
			IAR_ATTR_PACKED
			struct TASKING_ATTR_PACKED USB_Descriptor_Interface_Association
			{
				/**< Descriptor header, including type and size. */
				USB_Descriptor_Header_t Header;

				/**< Index of the first associated interface. */
				uint8_t FirstInterfaceIndex;

				/**< Total number of associated interfaces. */
				uint8_t TotalInterfaces;

				/**< Interface class ID. */
				uint8_t Class;

				/**< Interface subclass ID. */
				uint8_t SubClass;

				/**< Interface protocol ID. */
				uint8_t Protocol;

				/**< Index of the string descriptor describing the interface
				 * association.*/
				uint8_t IADStrIndex;
			}ATTR_PACKED;
			typedef struct USB_Descriptor_Interface_Association
							USB_Descriptor_Interface_Association_t;

			/** \brief Standard USB Interface Association Descriptor (USB-IF
			 * naming conventions).
			 *
			 *  Type define for a standard Interface Association Descriptor.
			 *  This structure uses the relevant standard's given
			 *  element names to ensure compatibility with the standard.
			 *
			 *  This descriptor has been added as a supplement to the USB2.0
			 *  standard, in the ECN located at <a>http://www.usb.org/developers
			 *  /docs/InterfaceAssociationDescriptor_ecn.pdf</a>. It allows
			 *  composite devices with multiple interfaces related to the same
			 *  function to have the multiple interfaces bound together at the
			 *  point of enumeration, loading one generic driver for all the
			 *  interfaces in the single function.
			 *
			 *  \see \ref USB_Descriptor_Interface_Association_t for the version
			 *   of this type with non-standard LUFA specific  element names.
			 *
			 *  \note Regardless of CPU architecture, these values should be
			 *  stored as little endian.
			 */
			IAR_ATTR_PACKED
			struct TASKING_ATTR_PACKED USB_StdDescriptor_Interface_Association
			{
				/**< Size of the descriptor, in bytes. */
				uint8_t bLength;

				/**< Type of the descriptor, either a value in
				 * \ref USB_DescriptorTypes_t or a value given by the specific
				 * class.*/
				uint8_t bDescriptorType;

				/**< Index of the first associated interface. */
				uint8_t bFirstInterface;

				/**< Total number of associated interfaces. */
				uint8_t bInterfaceCount;

				/**< Interface class ID. */
				uint8_t bFunctionClass;

				/**< Interface subclass ID. */
				uint8_t bFunctionSubClass;

				/**< Interface protocol ID. */
				uint8_t bFunctionProtocol;

				/**< Index of the string descriptor describing the interface
				 * association.*/
				uint8_t iFunction;
			}ATTR_PACKED;
			typedef struct USB_StdDescriptor_Interface_Association
							USB_StdDescriptor_Interface_Association_t;

			/** \brief Standard USB Endpoint Descriptor (LUFA naming
			 * conventions).
			 *
			 *  Type define for a standard Endpoint Descriptor. This structure
			 *  uses LUFA-specific element names to make each element's purpose
			 *  clearer.
			 *
			 *  \see \ref USB_StdDescriptor_Endpoint_t for the version of this
			 *  type with standard element names.
			 *
			 *  \note Regardless of CPU architecture, these values should be
			 *  stored as little endian.
			 */
			IAR_ATTR_PACKED struct TASKING_ATTR_PACKED USB_Descriptor_Endpoint
			{
				/**< Descriptor header, including type and size. */
				USB_Descriptor_Header_t Header;

				/**< Logical address of the endpoint within the device for the
				 * current  configuration, including direction mask.*/
				uint8_t  EndpointAddress;

				/**< Endpoint attributes, comprised of a mask of the endpoint
				 * type (EP_TYPE_*)  and attributes (ENDPOINT_ATTR_*) masks. */
				uint8_t  Attributes;

				/**< Size of the endpoint bank, in bytes. This indicates the
				 * maximum packet size that the endpoint can receive at a
				 * time.*/
				uint16_t EndpointSize;

				/**< Polling interval in milliseconds for the endpoint if it is
				 * an INTERRUPT or ISOCHRONOUS type.*/
				uint8_t  PollingIntervalMS;
			}ATTR_PACKED;
			typedef struct USB_Descriptor_Endpoint USB_Descriptor_Endpoint_t;

			/** \brief Standard USB Endpoint Descriptor (USB-IF naming
			 * conventions).
			 *
			 *  Type define for a standard Endpoint Descriptor. This structure
			 *  uses the relevant standard's given element names to ensure
			 *  compatibility with the standard.
			 *
			 *  \see \ref USB_Descriptor_Endpoint_t for the version of this type
			 *   with non-standard LUFA specific element names.
			 *
			 *  \note Regardless of CPU architecture, these values should be
			 *  stored as little endian.
			 */
			IAR_ATTR_PACKED
			struct TASKING_ATTR_PACKED USB_StdDescriptor_Endpoint
			{
				/**< Size of the descriptor, in bytes. */
				uint8_t  bLength;

				/**< Type of the descriptor, either a value in
				 * \ref USB_DescriptorTypes_t or a value given by the specific
				 * class. */
				uint8_t  bDescriptorType;

				/**< Logical address of the endpoint within the device for the
				 * current configuration, including direction mask.*/
				uint8_t  bEndpointAddress;

				/**< Endpoint attributes, comprised of a mask of the endpoint
				 * type (EP_TYPE_*) and attributes (ENDPOINT_ATTR_*) masks. */
				uint8_t  bmAttributes;

				/**< Size of the endpoint bank, in bytes. This indicates the
				 * maximum packet size that the endpoint can receive at a
				 * time.*/
				uint16_t wMaxPacketSize;

				/**< Polling interval in milliseconds for the endpoint if it is
				 * an INTERRUPT or ISOCHRONOUS type.*/
				uint8_t  bInterval;
			}ATTR_PACKED;
			typedef struct USB_StdDescriptor_Endpoint
								USB_StdDescriptor_Endpoint_t;

			/** \brief Standard USB String Descriptor (LUFA naming conventions).
			 *
			 *  Type define for a standard string descriptor. Unlike other
			 *  standard descriptors, the length of the descriptor for placement
			 *   in the descriptor header must be determined by the
			 *   \ref USB_STRING_LEN() macro rather than by the size of the
			 *   descriptor structure, as the length is not fixed.
			 *
			 *  This structure should also be used for string index 0, which
			 *  contains the supported language IDs for the device as an array.
			 *
			 *  This structure uses LUFA-specific element names to make each
			 *  element's purpose clearer.
			 *
			 *  \see \ref USB_StdDescriptor_String_t for the version of this
			 *  type with standard element names.
			 *
			 *  \note Regardless of CPU architecture, these values should be
			 *  stored as little endian.
			 */
			IAR_ATTR_PACKED struct TASKING_ATTR_PACKED USB_Descriptor_String
			{
				/**< Descriptor header, including type and size. */
				USB_Descriptor_Header_t Header;

				/**< String data, as unicode characters (alternatively, string
				 * language IDs). If normal ASCII characters are to be used,
				 * they must be added as an array of characters rather than a
				 * normal C string so that they are widened to Unicode size.
				 * Under GCC, strings prefixed with the "L" character (before
				 * the opening string quotation mark) are considered to be
				 * Unicode strings, and may be used instead of an explicit array
				 *  of ASCII characters on little endian devices with\c wchar_t
				 *  encoding.*/
				uint16_t UnicodeString[50];
			}ATTR_PACKED;
			typedef struct USB_Descriptor_String USB_Descriptor_String_t;

			/** \brief Standard USB String Descriptor (USB-IF naming
			 * conventions).
			 *
			 *  Type define for a standard string descriptor. Unlike other
			 *  standard descriptors, the length
			 *  of the descriptor for placement in the descriptor header must be
			 *   determined by the \ref USB_STRING_LEN() macro rather than by
			 *   the size of the descriptor structure, as the length is not
			 *   fixed.
			 *
			 *  This structure should also be used for string index 0, which
			 *  contains the supported language IDs for the device as an array.
			 *
			 *  This structure uses the relevant standard's given element names
			 *  to ensure compatibility with the standard.
			 *
			 *  \see \ref USB_Descriptor_String_t for the version of this type
			 *  with with non-standard LUFA specific element names.
			 *
			 *  \note Regardless of CPU architecture, these values should be
			 *  stored as little endian.
			 */
			IAR_ATTR_PACKED struct TASKING_ATTR_PACKED USB_StdDescriptor_String
			{
				/**< Size of the descriptor, in bytes. */
				uint8_t bLength;

				/**< Type of the descriptor, either a value in
				 * \ref USB_DescriptorTypes_t or a value given by the specific
				 * class.*/
				uint8_t bDescriptorType;

				/**< String data, as unicode characters (alternatively, string
				 * language IDs).If normal ASCII characters are to be used, they
				 *  must be added as an array of characters rather than a normal
				 *   C string so that they are widened to Unicode size. Under
				 *   GCC, strings prefixed with the "L" character (before the
				 *   opening string quotation mark) are considered to be Unicode
				 *    strings, and may be used instead of an explicit array of
				 *    ASCII characters. */
				uint16_t bString[50];
			}ATTR_PACKED;
			typedef struct USB_StdDescriptor_String USB_StdDescriptor_String_t;

	/* Private Interface - For use in library only: */
	#if !defined(__DOXYGEN__)
		/* Macros: */
			#define VERSION_TENS(x)                   (int)((int)(x) / 10)
			#define VERSION_ONES(x)                   (int)((int)(x) % 10)
			#define VERSION_TENTHS(x)                 (int)((x - (int)x) * 10)
			#define VERSION_HUNDREDTHS(x)             \
										(int)((x * 100) - ((int)(x * 10) * 10))
	#endif

	/* Disable C linkage for C++ Compilers: */
		#if defined(__cplusplus)
			}
		#endif

#endif

/** @} */

