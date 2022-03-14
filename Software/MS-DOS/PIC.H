

#ifndef _PICINC
#define _PICINC

//
// Defines for the PIC chip.
//

#define PIC1_ICR 0x20	        // 8259 PIC Interrupt Control Register (ICR)

#define PIC2_ICR 0xA0		// 8259 PIC Interrupt Control Register (ICR)

#define PIC1_IMR 0x21	        // 8259 PIC Interrupt Mask Register (IMR)

#define PIC2_IMR 0xA1		// 8259 PIC Interrupt Mask Register (IMR)


#define IMR_ENABLE_IRQ0  0xFE   // Enables IRQ0 when ANDed with the PIC1_IMR

#define IMR_DISABLE_IRQ0 0x01   // Disables IRQ0 when OR'd with the PIC1_IMR

#define IMR_ENABLE_IRQ1  0xFD   // Enables IRQ1 when ANDed with the PIC1_IMR

#define IMR_DISABLE_IRQ1 0x02   // Disables IRQ1 when OR'd with the PIC1_IMR

#define IMR_ENABLE_IRQ2  0xFB   // Enables IRQ2 when ANDed with the PIC1_IMR

#define IMR_DISABLE_IRQ2 0x04   // Disables IRQ2 when OR'd with the PIC1_IMR

#define IMR_ENABLE_IRQ3  0xF7   // Enables IRQ3 when ANDed with the PIC1_IMR

#define IMR_DISABLE_IRQ3 0x08   // Disables IRQ3 when OR'd with the PIC1_IMR

#define IMR_ENABLE_IRQ4  0xEF   // Enables IRQ4 when ANDed with the PIC1_IMR

#define IMR_DISABLE_IRQ4 0x10   // Disables IRQ4 when OR'd with the PIC1_IMR

#define IMR_ENABLE_IRQ5  0xDF   // Enables IRQ5 when ANDed with the PIC1_IMR

#define IMR_DISABLE_IRQ5 0x20   // Disables IRQ5 when OR'd with the PIC1_IMR

#define IMR_ENABLE_IRQ6  0xBF   // Enables IRQ6 when ANDed with the PIC1_IMR

#define IMR_DISABLE_IRQ6 0x40   // Disables IRQ6 when OR'd with the PIC1_IMR

#define IMR_ENABLE_IRQ7  0x7F   // Enables IRQ7 when ANDed with the PIC1_IMR

#define IMR_DISABLE_IRQ7 0x80   // Disables IRQ7 when OR'd with the PIC1_IMR

//
// The following are found in PIC 2
//

#define IMR_ENABLE_IRQ8   0xFE   // Enables IRQ8 when ANDed with the PIC2_IMR

#define IMR_DISABLE_IRQ8  0x01   // Disables IRQ8 when OR'd with the PIC2_IMR

#define IMR_ENABLE_IRQ9   0xFD   // Enables IRQ9 when ANDed with the PIC2_IMR

#define IMR_DISABLE_IRQ9  0x02   // Disables IRQ9 when OR'd with the PIC2_IMR

#define IMR_ENABLE_IRQ10  0xFB   // Enables IRQ10 when ANDed with the PIC2_IMR

#define IMR_DISABLE_IRQ10 0x04   // Disables IRQ10 when OR'd with the PIC2_IMR

#define IMR_ENABLE_IRQ11  0xF7   // Enables IRQ11 when ANDed with the PIC2_IMR

#define IMR_DISABLE_IRQ11 0x08   // Disables IRQ11 when OR'd with the PIC2_IMR

#define IMR_ENABLE_IRQ12  0xEF   // Enables IRQ12 when ANDed with the PIC2_IMR

#define IMR_DISABLE_IRQ12 0x10   // Disables IRQ12 when OR'd with the PIC2_IMR

#define IMR_ENABLE_IRQ13  0xDF   // Enables IRQ13 when ANDed with the PIC2_IMR

#define IMR_DISABLE_IRQ13 0x20   // Disables IRQ13 when OR'd with the PIC2_IMR

#define IMR_ENABLE_IRQ14  0xBF   // Enables IRQ14 when ANDed with the PIC2_IMR

#define IMR_DISABLE_IRQ14 0x40   // Disables IRQ14 when OR'd with the PIC2_IMR

#define IMR_ENABLE_IRQ15  0x7F   // Enables IRQ15 when ANDed with the PIC2_IMR

#define IMR_DISABLE_IRQ15 0x80   // Disables IRQ15 when OR'd with the PIC2_IMR



#define PIC_GENERAL_EOI  0x20           // 8259 PIC command to signal a 
                                        // general End Of Interrupt

#define PIC_SPECIFIC_EOI_IRQ4 0x64      // 8259 PIC command to signal the
                                        // specific End Of Interrupt for IRQ4

#define PIC_SPECIFIC_EOI_IRQ3 0x63      // 8259 PIC command to signal the
                                        // specific End Of Interrupt for IRQ3

#define PIC_SPECIFIC_EOI_IRQ2 0x62	// 8259 PIC command to signal the
                                        // specific End Of Interrupt for IRQ2

#define PIC_SPECIFIC_EOI_IRQ8 0x60      // 8259 PIC command to signal the
                                        // specific End Of Interrupt for IRQ8

#define PIC_SPECIFIC_EOI_IRQ9 0x61	// 8259 PIC command to signal the
                                        // specific End Of Interrupt for IRQ9

#define IRQ0_ISR_VECTOR         0x08    // IRQ0 ISR vector
#define IRQ1_ISR_VECTOR         0x09    // IRQ1 ISR vector
#define IRQ2_ISR_VECTOR         0x0A    // IRQ2 ISR vector
#define IRQ3_ISR_VECTOR         0x0B    // IRQ3 ISR vector
#define IRQ4_ISR_VECTOR         0x0C    // IRQ4 ISR vector
#define IRQ5_ISR_VECTOR         0x0D    // IRQ5 ISR vector  
#define IRQ6_ISR_VECTOR         0x0E    // IRQ6 ISR vector
#define IRQ7_ISR_VECTOR         0x0F    // IRQ7 ISR vector

#define IRQ8_ISR_VECTOR         0x70    // IRQ8 ISR vector
#define IRQ9_ISR_VECTOR         0x71    // IRQ9 ISR vector
#define IRQ10_ISR_VECTOR        0x72    // IRQ10 ISR vector
#define IRQ11_ISR_VECTOR        0x73    // IRQ11 ISR vector
#define IRQ12_ISR_VECTOR        0x74    // IRQ12 ISR vector
#define IRQ13_ISR_VECTOR        0x75    // IRQ13 ISR vector
#define IRQ14_ISR_VECTOR        0x76    // IRQ14 ISR vector
#define IRQ15_ISR_VECTOR        0x77    // IRQ15 ISR vector

#endif // _PICINC
