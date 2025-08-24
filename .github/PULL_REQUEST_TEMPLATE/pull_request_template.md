# Pull Request Template

## Description
**Provide a clear and concise description of the changes in this PR.**

## Type of Change
**Select the type of change this PR represents:**
- [ ] Bug fix (non-breaking change which fixes an issue)
- [ ] New feature (non-breaking change which adds functionality)
- [ ] Breaking change (fix or feature that would cause existing functionality to not work as expected)
- [ ] Documentation update
- [ ] Configuration Item (CI) addition/modification
- [ ] Security enhancement
- [ ] Performance improvement
- [ ] Refactoring (no functional changes)

## Domain and Category Impact
**Select affected domains:**
- [ ] AIR
- [ ] SPACE
- [ ] DEFENSE
- [ ] GROUND
- [ ] CROSS

**Select affected categories:**
- [ ] A - Airframes
- [ ] M - Mechanical
- [ ] E - Environmental
- [ ] D - Digital/Distributed
- [ ] E - Energy
- [ ] O - Operating Systems
- [ ] P - Propulsion

## Lifecycle Phase Impact
**Select affected lifecycle phases:**
- [ ] 01-REQUIREMENTS
- [ ] 02-DESIGN
- [ ] 03-BUILDING-PROTOTYPING
- [ ] 04-EXECUTABLES-PACKAGES
- [ ] 05-VERIFICATION-VALIDATION
- [ ] 06-INTEGRATION-QUALIFICATION
- [ ] 07-CERTIFICATION-SECURITY
- [ ] 08-PRODUCTION-SCALE
- [ ] 09-OPS-SERVICES
- [ ] 10-MRO
- [ ] 11-SUSTAINMENT-RECYCLE

## Related Issues
**Link to related issues:**
- Fixes #(issue number)
- Relates to #(issue number)
- Blocked by #(issue number)

## Testing
**Describe the tests you ran to verify your changes:**
- [ ] Unit tests added/updated
- [ ] Integration tests added/updated
- [ ] Manual testing performed
- [ ] Performance testing completed
- [ ] Security testing performed
- [ ] Regression testing completed

**Test Coverage:**
- Previous coverage: __%
- New coverage: __%
- Target coverage: 90%

## Standards Compliance
**Verify compliance with applicable standards:**

**Code Quality:**
- [ ] Follows coding standards
- [ ] Passes linting checks
- [ ] Includes appropriate comments
- [ ] No hardcoded values
- [ ] Error handling implemented

**Aviation Standards (if applicable):**
- [ ] DO-178C compliance verified
- [ ] DO-254 compliance verified
- [ ] ARP4754A requirements met
- [ ] Safety requirements addressed

**Space Standards (if applicable):**
- [ ] ECSS compliance verified
- [ ] NASA standards met
- [ ] Radiation tolerance considered

**Defense Standards (if applicable):**
- [ ] MIL-STD compliance verified
- [ ] Cybersecurity requirements met
- [ ] ITAR/EAR compliance checked

## Security Review
**Security considerations:**
- [ ] No sensitive information exposed
- [ ] Authentication/authorization unchanged or improved
- [ ] Input validation implemented
- [ ] SQL injection prevention verified
- [ ] XSS prevention implemented
- [ ] CSRF protection maintained
- [ ] Encryption requirements met

**Classification Level:**
- [ ] Public
- [ ] Proprietary
- [ ] Export Controlled
- [ ] Classified

## Configuration Management
**Configuration management checklist:**
- [ ] Configuration Item(s) properly identified
- [ ] Version control applied correctly
- [ ] Change documentation complete
- [ ] Traceability maintained
- [ ] Impact analysis performed
- [ ] Approval process followed

## Deployment
**Deployment considerations:**
- [ ] Database migrations included
- [ ] Environment variables documented
- [ ] Backward compatibility maintained
- [ ] Rollback plan available
- [ ] Monitoring/alerting updated

## Documentation
**Documentation updates:**
- [ ] Code comments added/updated
- [ ] API documentation updated
- [ ] User documentation updated
- [ ] Architecture documentation updated
- [ ] Deployment documentation updated
- [ ] Troubleshooting guide updated

## Review Checklist
**For reviewers:**
- [ ] Code changes are minimal and focused
- [ ] Changes align with requirements
- [ ] Security implications reviewed
- [ ] Performance impact assessed
- [ ] Documentation is adequate
- [ ] Tests are comprehensive
- [ ] Compliance requirements met

## Breaking Changes
**If this is a breaking change, describe:**
- What breaks
- Migration path for users
- Timeline for deprecation
- Communication plan

## Performance Impact
**Performance considerations:**
- [ ] No performance degradation
- [ ] Performance improvement measured
- [ ] Load testing performed (if needed)
- [ ] Memory usage assessed
- [ ] Database impact evaluated

## Additional Notes
**Any additional information for reviewers:**

---

## Reviewer Assignment
**Required reviewers:**
- [ ] Technical Lead: @username
- [ ] Domain Expert: @username  
- [ ] Security Review: @username (if security-related)
- [ ] Configuration Manager: @username (if CI changes)

**Review timeline:** 
- Target review completion: [Date]
- Deployment target: [Date]

---

**Note:** By submitting this PR, I confirm that my contribution is made under the terms of the project license and complies with export control regulations.