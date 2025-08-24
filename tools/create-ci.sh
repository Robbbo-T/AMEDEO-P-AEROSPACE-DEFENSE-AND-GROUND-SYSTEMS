#!/bin/bash

# AMEDEO-P Framework Setup Script
# Creates a new Configuration Item (CI) structure with all 11 lifecycle phases

set -e

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Configuration
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
UTCS_ROOT="../UTCS"
TEMPLATES_ROOT="../templates"

# Function to display usage
usage() {
    echo -e "${BLUE}AMEDEO-P Configuration Item Creation Tool${NC}"
    echo ""
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -d, --domain DOMAIN     Domain (AIR|SPACE|DEFENSE|GROUND|CROSS)"
    echo "  -c, --category CAT      Category (A|M|E|D|E|O|P)"
    echo "  -s, --system ID         System ID (001-999)"
    echo "  -a, --assembly ID       Constituent Assembly ID (001-999)"
    echo "  -i, --ci ID            Configuration Item ID (001-999)"
    echo "  -n, --name NAME        Descriptive name for the CI"
    echo "  -t, --type TYPE        CI type (hardware|software|document)"
    echo "  -h, --help             Display this help message"
    echo ""
    echo "Examples:"
    echo "  $0 -d AIR -c A -s 001 -a 001 -i 001 -n \"Fuselage Skin Panel\" -t hardware"
    echo "  $0 -d AIR -c D -s 432 -a 432 -i 002 -n \"Flight Plan Engine\" -t software"
    echo ""
}

# Function to validate domain
validate_domain() {
    local domain=$1
    case $domain in
        AIR|SPACE|DEFENSE|GROUND|CROSS)
            return 0
            ;;
        *)
            echo -e "${RED}Error: Invalid domain '$domain'. Must be one of: AIR, SPACE, DEFENSE, GROUND, CROSS${NC}"
            return 1
            ;;
    esac
}

# Function to validate category
validate_category() {
    local category=$1
    case $category in
        A|M|E|D|O|P)
            return 0
            ;;
        *)
            echo -e "${RED}Error: Invalid category '$category'. Must be one of: A, M, E, D, O, P${NC}"
            return 1
            ;;
    esac
}

# Function to get category name
get_category_name() {
    local category=$1
    case $category in
        A) echo "Airframes" ;;
        M) echo "Mechanical" ;;
        E) echo "Environmental" ;;
        D) echo "Digital_Distributed" ;;
        O) echo "Operating_Systems" ;;
        P) echo "Propulsion" ;;
    esac
}

# Function to create directory structure
create_directory_structure() {
    local domain=$1
    local category=$2
    local system_id=$3
    local assembly_id=$4
    local ci_id=$5
    local ci_name=$6
    local ci_type=$7
    
    local category_name=$(get_category_name $category)
    local system_dir="System-${system_id}-${ci_name// /}"
    local assembly_dir="CA-${category}${category:+A}${system_id}"
    local ci_dir="CI-${category}${category:+A}${system_id}-${ci_id}"
    
    local full_path="${UTCS_ROOT}/${domain}/${category_name}/${system_dir}/${assembly_dir}/${ci_dir}"
    
    echo -e "${BLUE}Creating CI structure at: ${full_path}${NC}"
    
    # Create main directory
    mkdir -p "$full_path"
    
    # Create 11 lifecycle phase directories
    local phases=(
        "01-REQUIREMENTS"
        "02-DESIGN"
        "03-BUILDING-PROTOTYPING"
        "04-EXECUTABLES-PACKAGES"
        "05-VERIFICATION-VALIDATION"
        "06-INTEGRATION-QUALIFICATION"
        "07-CERTIFICATION-SECURITY"
        "08-PRODUCTION-SCALE"
        "09-OPS-SERVICES"
        "10-MRO"
        "11-SUSTAINMENT-RECYCLE-EOL"
    )
    
    for phase in "${phases[@]}"; do
        mkdir -p "${full_path}/${phase}"
        echo -e "${GREEN}Created phase: ${phase}${NC}"
        
        # Copy appropriate templates based on CI type and phase
        copy_templates "$full_path/$phase" "$phase" "$ci_type" "$ci_name"
    done
    
    # Create CI metadata file
    create_ci_metadata "$full_path" "$domain" "$category" "$system_id" "$assembly_id" "$ci_id" "$ci_name" "$ci_type"
    
    echo -e "${GREEN}Successfully created CI: ${ci_dir}${NC}"
}

# Function to copy templates
copy_templates() {
    local target_dir=$1
    local phase=$2
    local ci_type=$3
    local ci_name=$4
    
    local template_dir="${TEMPLATES_ROOT}/${phase}"
    
    if [ -d "$template_dir" ]; then
        # Copy common templates
        if [ -f "${template_dir}/README.md" ]; then
            sed "s/{{CI_NAME}}/${ci_name}/g; s/{{CI_TYPE}}/${ci_type}/g" \
                "${template_dir}/README.md" > "${target_dir}/README.md"
        fi
        
        # Copy type-specific templates
        if [ -d "${template_dir}/${ci_type}" ]; then
            cp -r "${template_dir}/${ci_type}/"* "$target_dir/" 2>/dev/null || true
        fi
    fi
}

# Function to create CI metadata
create_ci_metadata() {
    local ci_path=$1
    local domain=$2
    local category=$3
    local system_id=$4
    local assembly_id=$5
    local ci_id=$6
    local ci_name=$7
    local ci_type=$8
    
    cat > "${ci_path}/CI-METADATA.yaml" << EOF
# AMEDEO-P Configuration Item Metadata
# This file contains the essential metadata for the Configuration Item

ci_info:
  identifier: "CI-${category}${category:+A}${system_id}-${ci_id}"
  name: "${ci_name}"
  type: "${ci_type}"
  version: "1.0.0"
  created_date: "$(date -u +%Y-%m-%dT%H:%M:%SZ)"
  
hierarchy:
  domain: "${domain}"
  category: "${category}"
  category_name: "$(get_category_name $category)"
  system_id: "${system_id}"
  assembly_id: "${assembly_id}"
  ci_id: "${ci_id}"

lifecycle:
  current_phase: "01-REQUIREMENTS"
  phases_completed: []
  phases_pending:
    - "01-REQUIREMENTS"
    - "02-DESIGN"
    - "03-BUILDING-PROTOTYPING"
    - "04-EXECUTABLES-PACKAGES"
    - "05-VERIFICATION-VALIDATION"
    - "06-INTEGRATION-QUALIFICATION"
    - "07-CERTIFICATION-SECURITY"
    - "08-PRODUCTION-SCALE"
    - "09-OPS-SERVICES"
    - "10-MRO"
    - "11-SUSTAINMENT-RECYCLE-EOL"

standards:
  applicable_standards:
    - "ISO-9001"
    - "AS-9100"
  domain_specific_standards:
    air:
      - "DO-178C"
      - "DO-254"
      - "ARP4754A"
    space:
      - "ECSS-E-ST-40C"
      - "ECSS-Q-ST-80C"
    defense:
      - "MIL-STD-882E"
      - "MIL-STD-498"
    ground:
      - "ICAO-Annex-14"
      - "IATA-ISAGO"

traceability:
  parent_system: "System-${system_id}"
  parent_assembly: "CA-${category}${category:+A}${system_id}"
  interfaces: []
  dependencies: []
  requirements_source: "TBD"

classification:
  security_level: "Proprietary"
  export_control: "TBD"
  handling_instructions: "Standard proprietary handling"

contacts:
  responsible_engineer: "TBD"
  configuration_manager: "TBD"
  quality_assurance: "TBD"
  
maintenance:
  last_updated: "$(date -u +%Y-%m-%dT%H:%M:%SZ)"
  update_frequency: "As needed"
  next_review_date: "$(date -u -d '+3 months' +%Y-%m-%dT%H:%M:%SZ)"
EOF

    echo -e "${GREEN}Created CI metadata file${NC}"
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -d|--domain)
            DOMAIN="$2"
            shift 2
            ;;
        -c|--category)
            CATEGORY="$2"
            shift 2
            ;;
        -s|--system)
            SYSTEM_ID="$2"
            shift 2
            ;;
        -a|--assembly)
            ASSEMBLY_ID="$2"
            shift 2
            ;;
        -i|--ci)
            CI_ID="$2"
            shift 2
            ;;
        -n|--name)
            CI_NAME="$2"
            shift 2
            ;;
        -t|--type)
            CI_TYPE="$2"
            shift 2
            ;;
        -h|--help)
            usage
            exit 0
            ;;
        *)
            echo -e "${RED}Unknown option: $1${NC}"
            usage
            exit 1
            ;;
    esac
done

# Validate required parameters
if [ -z "$DOMAIN" ] || [ -z "$CATEGORY" ] || [ -z "$SYSTEM_ID" ] || [ -z "$ASSEMBLY_ID" ] || [ -z "$CI_ID" ] || [ -z "$CI_NAME" ] || [ -z "$CI_TYPE" ]; then
    echo -e "${RED}Error: All parameters are required${NC}"
    usage
    exit 1
fi

# Validate inputs
validate_domain "$DOMAIN" || exit 1
validate_category "$CATEGORY" || exit 1

# Validate numeric IDs
if ! [[ "$SYSTEM_ID" =~ ^[0-9]{3}$ ]] || ! [[ "$ASSEMBLY_ID" =~ ^[0-9]{3}$ ]] || ! [[ "$CI_ID" =~ ^[0-9]{3}$ ]]; then
    echo -e "${RED}Error: System, Assembly, and CI IDs must be 3-digit numbers (001-999)${NC}"
    exit 1
fi

# Validate CI type
case $CI_TYPE in
    hardware|software|document)
        ;;
    *)
        echo -e "${RED}Error: CI type must be one of: hardware, software, document${NC}"
        exit 1
        ;;
esac

# Create the directory structure
create_directory_structure "$DOMAIN" "$CATEGORY" "$SYSTEM_ID" "$ASSEMBLY_ID" "$CI_ID" "$CI_NAME" "$CI_TYPE"

echo -e "${GREEN}Configuration Item creation completed successfully!${NC}"